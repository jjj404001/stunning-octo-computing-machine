/*
Implementation of lz77 encoding algorith.
*/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

    uint64_t window_index = 0;
    uint64_t view_index   = 0;

    // Fill view buffer with inputs.

    while(_size > view_size)
    {
        memcpy(view, &byte_src[view_index], view_size); // use byte_src as view buffer.

        

        
        _size -= view_size;
    }





    free(window);
    free(view);
}