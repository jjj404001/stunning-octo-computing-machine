#ifndef _DEFLATE_FORMAT_H
#define _DEFLATE_FORMAT_H

#define BTYPE_NOCOMP 0b00
#define BTYPE_FIXED 0b01
#define BTYPE_DYNAMIC 0b10
#define BTYPE_ERROR 0b11

#define BIT_1(src, mask) (src) | (mask)
#define BIT_0(src, mask) (src) & ^(mask)

#define FIXED_END_OF_DATA 0b0000000
#define COMP_END_OF_DATA 0b100000000 // 256

#endif