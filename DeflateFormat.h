#ifndef _DEFLATE_FORMAT_H
#define _DEFLATE_FORMAT_H

#include <stdint.h>

#define BTYPE_NOCOMP 0b00
#define BTYPE_FIXED 0b01
#define BTYPE_DYNAMIC 0b10
#define BTYPE_ERROR 0b11

#define BIT_MASK_8 0b11111111
#define BIT_MASK_9 0b0000000111111111

#define FIXED_END_OF_DATA 0b0000000
#define COMP_END_OF_DATA 0b100000000 // 256

typedef struct _deflate_out_stream
{
    uint8_t BitMask;
    uint8_t BitUsed;
    uint8_t Data;
    uint8_t BlockStarted;
} DeflateOStream;

#endif