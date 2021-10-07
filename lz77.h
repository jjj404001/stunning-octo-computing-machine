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
        
        
        while(window_count > curr_count)
        {
            int i = 0;
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
                new->Literal  = window[0]; // It have to be the p + length, wrong

                current->Next = new;
                current = new;
            }
            else
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length = 0;
                new->Distance = window_count - i;
                new->Literal  = window[i]; // wrong


                int j = 0;

                while(window[i++] == view[j++]) // todo : add some barrier to prevent out of bound when i or j gets out of the window or view buffer
                    ++new->Length;              // also, need Literal have to be valid byte, force quit before the end of buffer

                current->Next = new;
                current = new;
            }

            window[curr_count++] = view[0];
            memcpy(view, &byte_src[++src_index], view_size);
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