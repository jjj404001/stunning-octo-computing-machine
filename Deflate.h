#ifndef _DEFLATE_H
#define _DEFLATE_H

#include <stdio.h>
#include "lz77.h"
#include "DeflateFormat.h"
#include "FixedHuffman.h"

void WriteDeflateTypeBits(uint8_t* _type, uint8_t* _data, uint8_t* _bit_used)
{
    // Put type bit
    *_data     |= BTYPE_FIXED >> (8 - *_bit_used);
    *_bit_used += 2;
}

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
            WriteDeflateTypeBits(BTYPE_FIXED, ostream.Data, &ostream.BitUsed);

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
            uint8_t shifted = 0xFF & (huff >> (ostream.BitUsed + (huff_bit_count -8))); // shift "BitUsed" amount, +1 if it is 9bits.
            ostream.Data    |= shifted;
            ostream.BitUsed += huff_bit_count;
        }
        else
        {
            // Compressed
        }

        if(ostream.BitUsed >= 8)
        {
            fwrite(&ostream.Data, 1, 1, deflate_out);

            uint8_t num = ostream.BitUsed - 8;
            ostream.BitUsed = num;

            uint8_t shifted = huff & ((1 << (ostream.BitUsed +1)) -1);
            shifted = shifted << (8 - num);
            
            ostream.Data = shifted;
        }

        node = node->Next;   
    }

    fclose(deflate_out);
}

#endif