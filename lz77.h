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
    const size_t buffer_count = window_count + view_count;

    const size_t window_size = window_count * sizeof(uint8_t);
    const size_t view_size   = view_count   * sizeof(uint8_t);
    const size_t buffer_size = window_size + view_size;
    
    uint8_t* buffer = (uint8_t*)malloc(window_size + view_size);
    uint8_t* window = &buffer[0];
    uint8_t* view   = &buffer[window_count]; 

    uint64_t src_index    = 0;
    



    LzLinkedlist linked_list;
    linked_list.Head = (LzNode*)malloc(sizeof(LzNode));

    LzNode* current = linked_list.Head;

    uint64_t view_start = 0;
    uint64_t view_end   = 0;
    uint64_t window_start = window_count -1;
    uint64_t window_end   = 0;

    // Fill view buffer with inputs.
    {
        size_t curr_view_count = view_count;
        // First LDD is always be 0,0,view[0]
        current->Length   = 0;
        current->Distance = 0;
        current->Literal  = byte_src[0];

        uint64_t curr_count = 1;

        memcpy(view, &byte_src[1], view_size); // start from 1st data, since 0st data is gonna stored in window[0]
        window[window_start] = current->Literal;
        
        // first iteration to fill up the window buffer
        while(curr_count < window_count)
        {
            uint64_t i = window_start;

            while(i < window_count)
            {
                if(window[i] == view[0])
                    break;
                i++;
            }

            if (i == window_count) // if i is same with current window size, no p found inside window buffer.
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
                if(curr_count > curr_view_count) // TODO: use MIN here.
                    max_it = curr_view_count;

                uint64_t j = 0;
                while(max_it-- && window[i++] == view[j++])
                    ++new->Length;

                if(new->Length == curr_view_count)
                {
                    uint64_t new_src_index = src_index + new->Length - window_start;

                    if(buffer_count > _size - new_src_index)// if it goes out of the source size
                    {
                        uint64_t diff = buffer_count - (_size - new_src_index);
                        curr_view_count -= diff;
                        memcpy(buffer, &byte_src[new_src_index], buffer_count-diff);
                    }
                    else
                    {
                        memcpy(buffer, &byte_src[new_src_index], buffer_count);
                        new->Literal = view[-1]; // last element of window
                        current->Next = new;
                        current = new;

                        curr_count = window_count;
                        window_start = 0;
                    }
                    
                    src_index = new_src_index;
                    continue;
                }
                else
                    new->Literal = view[new->Length];

                current->Next = new;
                current = new;
            }


            curr_count += current->Length +1;
            window_start -= current->Length +1;
            //
            memcpy(&window[window_start], &byte_src[src_index], window_count + curr_view_count);
        }
    }

    uint64_t min_count = view_count;
    if(min_count > window_count) 
        min_count = window_count;

     
    while(src_index < _size)
    {
        uint64_t i = window_start;
        while(i < window_count)
        {
            if(window[i] == view[0])
                break;
            i++;
        }

        if (i == window_count) // if i is same with current window size, no p found inside window buffer.
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

            uint64_t max_it = min_count;

            uint64_t j = 0;
            while(max_it-- && window[i++] == view[j++])
                ++new->Length;

            // If src_index goes out of the src.
            if(src_index + window_count + new->Length > _size)
            {
                
            }
            else
                new->Literal = view[new->Length];

            current->Next = new;
            current = new;
        }

        src_index += current->Length +1;
        memcpy(buffer, &byte_src[src_index], buffer_size);
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