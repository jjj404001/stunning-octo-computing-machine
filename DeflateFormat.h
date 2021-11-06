#ifndef _DEFLATE_FORMAT_H
#define _DEFLATE_FORMAT_H

#include <stdint.h>

#define BTYPE_NOCOMP 0b00
#define BTYPE_FIXED 0b01
#define BTYPE_DYNAMIC 0b10
#define BTYPE_ERROR 0b11

#define GETLEN(x)  0b11000000 & (x)
#define GETNLEN(x) 0b00110000 & (x)

typedef struct _deflate_header
{
    uint8_t Type;
    uint8_t* Data;
} DeflateHeader; 

#endif