/*
Implementation of lz77 encoding algorith.
*/
#include <stdlib.h>
#include <stdint.h>

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

    const size_t window_size = 0x06;
    const size_t view_size   = 0x04;

    void* window = malloc(window_size);
    void* view   = malloc(0x04);

    while(_size > window_size)
    {
        uint8_t p; 
    }

    if(_size > 0)
    {

    }
}