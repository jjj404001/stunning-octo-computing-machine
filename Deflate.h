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

        if(node->Length == 0)
        {
            // Put raw byte
            uint16_t huff = FixedHuffmanCodeDeflate[node->Literal];
            uint8_t  huff_bit_count = 0;

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

            uint16_t mask = 1 << huff_bit_count;

            while(mask != 0)
            {
                ostream.Data |= (mask & huff) >> ostream.BitUsed++;
                mask = mask >> 1;
            }
        }



        if(ostream.BitUsed >= 8) 
            fwrite (&ostream.Data , 1, 1, deflate_out);
    }

    fclose(deflate_out);
}

#endif