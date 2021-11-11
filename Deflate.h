#ifndef _DEFLATE_H
#define _DEFLATE_H

#include <stdio.h>
#include "lz77.h"
#include "DeflateFormat.h"
#include "FixedHuffman.h"

void Deflate(const char* _out_path, LzLinkedlist* _lz)
{
    FILE* deflate_out = fopen(_out_path, "wb");
    LzNode* node = _lz->Head;

    DeflateOStream ostream;
    memset(&ostream, 0, sizeof(DeflateOStream));

    while(node)
    {
        // Assume that there are only fixed huffmancode.
        if(!ostream.BlockStarted)
        {
            // Put block header
        }

        uint16_t huff = 0;
        uint8_t  huff_bit_count = 0;
        if(node->Length == 0)
        {
            // Put raw byte
            huff = FixedHuffmanCodeDeflate[node->Literal];
            

            // Should be single byte since the length is 0
            /*if (node->Literal >= 280)
                huff_bit_count = 8;
            else if (node->Literal >= 256)
                huff_bit_count = 7;
            else*/ 
            if (node->Literal >= 144)
                huff_bit_count = 9;
            else if (node->Literal >= 0)
                huff_bit_count = 8;

            
            uint8_t remaining_bits = 8 - ostream.BitUsed;
            uint8_t mask = 0;
            mask = ~mask;
            mask &= ((1 << (remaining_bits +1)) -1);

            ostream.Data |= (huff >> ostream.BitUsed) | ~mask;
            ostream.BitUsed += huff_bit_count;
        }

        if(ostream.BitUsed >= 8)
        {
            uint8_t num  = ostream.BitUsed - 8;
            ostream.BitUsed = num;
            huff = huff << (8 - num);
            num = 8 - num;
            uint16_t mask = ~((1 << (num +1)) -1); 
            huff &= mask;

            ostream.Data = huff & 0xFF;

            fwrite(&ostream.Data, 1, 1, deflate_out);
        }

        node = node->Next;   
    }

    fclose(deflate_out);
}

#endif