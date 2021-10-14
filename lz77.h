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

    if(_size < 1)
        return NULL;

    uint8_t* byte_src = (uint8_t*)_src;

    const size_t window_count = 0x06;
    const size_t view_count   = 0x04;

    const size_t window_size = window_count * sizeof(uint8_t);
    const size_t view_size   = view_count   * sizeof(uint8_t);
    const size_t buffer_size = window_size + view_size;
    
    uint8_t* buffer = (uint8_t*)malloc(window_size + view_size);
    uint8_t* window = &buffer[0];
    uint8_t* view   = &buffer[window_count]; 

    uint64_t src_index    = 0;
    
    uint64_t view_start = 0;
    uint64_t view_end   = 0;
    uint64_t window_start = window_count -1;
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

        uint64_t curr_count = 1;

        memcpy(view, &byte_src[++src_index], view_size); // start from 1st data, since 0st data is gonna stored in window[0]
        window[window_start] = current->Literal;
        
        // first iteration to fill up the window buffer
        while(curr_count < window_count)
        {
            uint64_t i = window_start;
            uint64_t search = 0; // count current searched element
            while(search < curr_count)
            {
                if(window[i] == view[0])
                    break;
                --i; // Iterate backward.
                ++search;
            }

            if (search == curr_count) // if search is same with current window size, no p found inside window buffer.
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length = 0;
                new->Distance = 0;
                new->Literal  = view[0];

                current->Next = new;
                current = new;
            }
            else
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length   = 0;
                new->Distance = window_count - i;

                uint64_t max_it = curr_count;
                if(curr_count > view_count) // TODO: use MIN here.
                    max_it = view_count;

                uint64_t j = 0;
                while(max_it-- && window[i++] == view[j++])
                    ++new->Length;

                if(new->Length < view_count)
                {
                    new->Literal  = view[new->Length];
                }
                else
                {
                    current->Next = new;
                    current = new;
                    continue;
                }

                
                current->Next = new;
                current = new;
            }


            memcpy(buffer, &byte_src[src_index -= current->Length +1], buffer_size);
            curr_count += current->Length +1;
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