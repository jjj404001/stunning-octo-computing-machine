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

void* EncodeLZ77(void* _src, size_t _size)
{
    /*
    TODO : find a way to return LDD.
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
    const size_t view_count   = 0x04;

    const size_t window_size = window_count * sizeof(uint8_t);
    const size_t view_size   = view_count   * sizeof(uint8_t);

    uint8_t* window = (uint8_t*)malloc(window_size);
    uint8_t* view   = (uint8_t*)malloc(view_size);

    uint64_t src_index    = 0;
    
    uint64_t view_start = 0;
    uint64_t view_end   = 0;
    uint64_t window_start = 0;
    uint64_t window_end   = 0;


    LzLinkedlist linked_list = {};
    linked_list.Head = (LzNode*)malloc(sizeof(LzNode));

    LzNode* current = linked_list.Head;

    // Fill view buffer with inputs.
    {
        // First LDD is always be 0,0,view[0]
        current->Length = 0;
        current->Distance = 0;
        current->Literal  = window[0];

        uint64_t curr_count = 0;

        memcpy(view, &byte_src[++src_index], view_size); // start from 1st data, since 0st data is gonna stored in window[0]
        window[curr_count++] = byte_src[0];
        
        
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
                new->Literal  = window[0];
                assigned = 1;

                current->Next = new;
                current = new;
            }
            else
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length = 0;
                new->Distance = window_count - i;


                uint64_t max_it = curr_count - i;
                if(max_it > view_count)
                    max_it = view_count;

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

            window[curr_count++] = view[0];
            memcpy(view, &byte_src[++src_index], view_size);

            if(!assigned)
                current->Literal = view[view_count - 1]; // todo : add boundary check, can be crash if view[] is out of byte src
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
            new->Literal  = window[0];
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
            if(max_it > view_count)
                max_it = view_count;

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
        memcpy(window, &window[1], window_size);
        window[window_size-1] = view[0];
        memcpy(view, &byte_src[++src_index], view_size);

        if(!assigned)
        {
            if(src_index == _size) // == if view[view_count - 1] goes out of boundary
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
    





    free(window);
    free(view);
}