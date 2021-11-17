#ifndef _DEFLATE_H
#define _DEFLATE_H

#include <stdio.h>
#include "lz77.h"
#include "heap.h"
#include "DeflateFormat.h"
#include "FixedHuffman.h"

void WriteDeflateTypeBits(uint8_t* _type, DeflateOStream* _out)
{
    // Put type bit
    _out->Data     |= BTYPE_FIXED >> (8 - _out->BitUsed);
    _out->BitUsed += 2;
}

void Deflate(const char* _out_path, LzLinkedlist* _lz)
{
    FILE* deflate_out = fopen(_out_path, "wb");
    LzNode* node = _lz->Head;

    DeflateOStream ostream;
    memset(&ostream, 0, sizeof(DeflateOStream));

    LzNode* stored_node = NULL;
    uint64_t node_count = 0;

    while(node)
    {
        uint16_t huff = 0;
        uint8_t  huff_bit_count = 0;

        if(stored_node == NULL)
            stored_node = node;

        ++node_count;

        if(node_count > HUFF_MAX_BIT_LENGTH)
        {
            node_count = 0;

            // Compress

            stored_node = NULL;
        }

        node = node->Next;
    }

    fclose(deflate_out);
}

#endif