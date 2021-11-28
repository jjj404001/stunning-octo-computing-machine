#ifndef _LZ77_H
#define _LZ77_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define MAX_D 0x8000

#define LZMIN(a, b) (((a) < (b)) ? (a) : (b))

typedef struct _lznode
{
    uint16_t Length;
    uint8_t  Literal; // Byte size literal
    uint16_t Distance;

    struct _lznode* Next;
} LzNode;

typedef struct _lzLinkedList
{
    uint64_t SizeInByte;
    uint64_t NodeCount;
    LzNode* Head;
} LzLinkedlist;

LzLinkedlist EncodeLZ77(const void* _src, const size_t _size);
uint8_t* DecodeFreeLZ77(LzLinkedlist _linked_list);
void FreeLZ77(LzLinkedlist _linked_list);
int SaveFreeLZ77(LzLinkedlist _linked_list, const char* _str);
#endif