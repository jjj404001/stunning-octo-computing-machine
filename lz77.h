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

    uint8_t* dp = (uint8_t*)calloc((window_count + 1) * (view_count + 1), sizeof(uint8_t));
    memset(dp, 0, sizeof(uint8_t) * (view_count + 1));

    LzLinkedlist linked_list = {};
    linked_list.Head = (LzNode*)malloc(sizeof(LzNode));

    LzNode* current = linked_list.Head;

    // Fill view buffer with inputs.
    {
        memcpy(view, byte_src, view_size); // Fill lockahead(view) buffer first
        window[0] = view[0];
        // First LDD is always be 0,0,view[0]
        current->Length = 0;
        current->Distance = 0;
        current->Literal  = window[0];

        uint64_t curr_count = 1;

        uint64_t max_i = 0;
        uint64_t max_j = 0;
        
        while(window_count > curr_count)
        {
            uint64_t i = 1;
            for (i; i < view_count+ 1; i++)
            {
                dp[(i * view_count)] = 0;

                uint64_t j = 1;
                for (j; j < curr_count+ 1; j++)
                {
                    if(view[i-1] == window[j-1])
                    {
                        dp[(i * view_count) + j] = dp[((i - 1) * view_count) + (j - 1)] + 1;

                        if(dp[(i * view_count) + j] > dp[(max_i * view_count) + max_j])
                        {
                            max_i = i;
                            max_j = j;
                        }
                    }
                }
            }

            curr_count++;
        }
    }
    


    while(_size > view_size)
    {
        memcpy(view, &byte_src[src_index], view_size); // use byte_src as view buffer.

        uint64_t view_index   = 0;
        uint64_t window_index = 0;
        while(view_index < view_count)
        {
            if(view[view_index] != window[window_index])
            {

            }

            view_index++;
        }


        src_index += view_size;
        _size -= view_size;
    }





    free(window);
    free(view);
}