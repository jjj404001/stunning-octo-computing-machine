/*
Implementation of lz77 encoding algorith.
*/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct _lznode
{
    uint64_t Length;
    uint8_t  Literal; // Byte size literal
    uint64_t Distance;

    struct _lznode* Next;
} LzNode;

typedef struct _lzLinkedList
{
    LzNode* Head;
} LzLinkedlist;

void* EncodeLZ77(const void* _src, const size_t _size)
{
    /*
    TODO : find a way to return LDD.
    TODO : We don't actually need two buffer, one big buffer with different entry
    TODO : With somekind of ring buffer or something..?
    View == Lockahead buffer
    Window == Search buffer

    peudo code

    1. fill view buffer with inputs.
    2. find longest common data string from windows.
        2-1. fill window buffer with data if there are no common data.
    Distance = distance from the start of common data to start of view buffer.
    Length   = length of common data.
    Literal  = data at the p + Length.

    need to revisit LCS...
    */

    uint8_t* byte_src = (uint8_t*)_src;

    const size_t window_count = 0x06;
    const size_t view_count   = 0x05;

    const size_t window_size = window_count * sizeof(uint8_t);
    const size_t view_size   = view_count   * sizeof(uint8_t);
    const size_t buffer_size = window_size + view_size;
    
    uint8_t* buffer = (uint8_t*)malloc(window_size + view_size);
    uint8_t* window = &buffer[0];
    uint8_t* view   = &buffer[window_count]; 

    uint64_t src_index    = 0;
    
    uint64_t view_start = 0;
    uint64_t view_end   = 0;
    uint64_t window_start = 0;
    uint64_t window_end   = 0;


    LzLinkedlist linked_list;
    linked_list.Head = (LzNode*)malloc(sizeof(LzNode));

    LzNode* current = linked_list.Head;

    // Fill view buffer with inputs.
    {
        // First LDD is always be 0,0,view[0]
        current->Length   = 0;
        current->Distance = 0;
        current->Literal  = byte_src[0];

        uint64_t curr_count = 0;

        memcpy(view, &byte_src[++src_index], view_size); // start from 1st data, since 0st data is gonna stored in window[0]
        window[curr_count++] = current->Literal;
        
        // first iteration to fill up the window buffer
        while(window_count > curr_count)
        {
            int assigned = 0;
            uint64_t i = 0;
            while(i < curr_count)
            {
                if(window[i] == view[0])
                    break;
                ++i;
            }

            if (i == curr_count) // if i is same with current window size, no p found inside window buffer.
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length = 0;
                new->Distance = 0;
                new->Literal  = view[0];
                assigned = 1;

                current->Next = new;
                current = new;
            }
            else
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length = 0;
                new->Distance = curr_count - i;


                uint64_t max_it = curr_count - i;
                if(max_it > view_count-1)
                    max_it = view_count-1;

                uint64_t j = 0;
                while(max_it-- && window[i++] == view[j++])
                    ++new->Length;              

                if(new->Length < view_count)
                {
                    assigned = 1;
                    new->Literal  = view[new->Length];
                }
                    

                current->Next = new;
                current = new;
            }


            if(current->Length >= view_size-1) // if length is bigger than actual view_size, than it means next p is out of boundary
            {
                uint64_t diff = curr_count + view_size; // in case if view buffer is a lot larger than window.
                uint64_t times = 0;
                while(diff > window_size)
                {
                    diff -= window_size;
                    ++times; // todo : use this one.
                }


                memcpy(window, &window[diff], curr_count - diff);
                memcpy(&window[curr_count - diff], view, view_size);
            }
            else
            {
                memcpy(&window[curr_count], view, current->Length +1);
            }
            memcpy(view, &byte_src[src_index += current->Length +1], view_size);
            curr_count += current->Length +1;
            

            if(!assigned)
            {
                if(src_index >= _size) // == if view[view_count - 1] goes out of boundary
                {
                    --current->Length;
                    --current->Distance;
                    current->Literal  = view[current->Length - 1];
                    // TODO : add last ldd here.
                }
                else
                    current->Literal = window[curr_count-1]; // it will going to be last element in window buffer,
                                                            // since current ldd comsumes entire view buffer.
            }
        }
    }
    


    while(src_index < _size)
    {
        int assigned = 0;
        uint64_t i = 0;
        while(i < window_count)
        {
            if(window[i] == view[0])
                break;
            ++i;
        }


        if (i == window_count) // if i is same with current window size, no p found inside window buffer.
        {
            LzNode* new = (LzNode*)malloc(sizeof(LzNode));
            new->Length = 0;
            new->Distance = 0;
            new->Literal  = view[0];
            assigned = 1;

            current->Next = new;
            current = new;
        }
        else
        {
            LzNode* new = (LzNode*)malloc(sizeof(LzNode));
            new->Length = 0;
            new->Distance = window_count - i;


            uint64_t max_it = window_count - i;
            if(max_it > view_count-1)
                max_it = view_count-1;

            uint64_t j = 0;
            while(max_it-- && window[i++] == view[j++])
                ++new->Length;              

            if(new->Length < view_count)
            {
                assigned = 1;
                new->Literal  = view[new->Length];
            }
                
            current->Next = new;
            current = new;
        }

        // todo : slide window
        //memcpy(window, &window[1], window_size);
        //window[window_size-1] = view[0];
        memcpy(buffer, &byte_src[src_index += current->Length +1], buffer_size);

        if(!assigned)
        {
            if(src_index >= _size) // == if view[view_count - 1] goes out of boundary
            {
                --current->Length;
                --current->Distance;
                current->Literal  = view[current->Length - 1];
            }
            else
                current->Literal = view[view_count - 1];
        }
    }

    current->Next = NULL;
    current = linked_list.Head;

    while(current)
    {
        printf("LDL : %d, %d, %c \n", current->Length, current->Distance, current->Literal);
        current = current->Next;
    }
    
    free(buffer);
}