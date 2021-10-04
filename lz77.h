/*
Implementation of lz77 encoding algorith.
*/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct LinkedList
{
    struct Node
    {
        uint64_t Length;
        uint8_t  Literal; // Byte size literal
        uint64_t Distance;
    };

    Node* Head;
};

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

    // Fill view buffer with inputs.
    {
        memcpy(view, byte_src, view_size); // Fill lockahead(view) buffer first
        window[0] = view[0];
        // First LDD is always be 0,0,view[0]
        // Maybe have to implement some kind of linked list for LDD.
        uint64_t curr_size = 1;


        while(window_size > curr_size)
        {
            
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