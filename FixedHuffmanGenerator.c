#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE* fout = fopen("FixedHuffman.h", "w");
    fputs("#ifndef _FIXED_HUFFMAN_H\n\0", fout);
    fputs("#define _FIXED_HUFFMAN_H\n\0", fout);
    fputs("#include <stdint.h>\n\0", fout);
    fputc('\n', fout);

    fputs("const uint16_t FixedHuffmanCodeDeflate[] = { \0", fout);

    uint16_t huffman = 0b00110000;    

    // 8 bits for 0~143
    int i = 0;
    for(i = 0; i < 144; i++)
    {
        uint16_t mask = 0b10000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        
        fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);

    huffman = 0b110010000;
    for(i = 144; i < 256; i++)
    {
        uint16_t mask = 0b100000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        
        fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);

    huffman = 0b00000000;
    for(i = 256; i < 279; i++)
    {
        uint16_t mask = 0b1000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        
        fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);

    huffman = 0b11000000;
    for(i = 280; i < 287; i++)
    {
        uint16_t mask = 0b10000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        
        if(i < 286)
            fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);

    fputs("};\n\0", fout);
    fputs("\n", fout);

    fputs("const uint16_t FixedHuffmanCodeInflate[] = { \0", fout);
    huffman = 256;
    for(i = 0; i < 24; i++)
    {
        uint16_t mask = 0b100000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);

    // Empty from 
    // 24~47
    for(i = 24; i < 48; i++)
    {
        fputs("0xFFFF, \0", fout);
    }
    fputc('\n', fout);

    huffman = 0;
    for(i = 0; i < 144; i++)
    {
        uint16_t mask = 0b100000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);

    // No empty chunk.

    huffman = 280;
    for(i = 0; i < 8; i++)
    {
        uint16_t mask = 0b100000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);
    // Empty from 
    // 200~399
    for(i = 200; i < 400; i++)
    {
        fputs("0xFFFF, \0", fout);
    }
    fputc('\n', fout);

    huffman = 144;
    for(i = 0; i < 112; i++)
    {
        uint16_t mask = 0b100000000;
        fputs("0b\0", fout);
        while(mask)
        {
            if(huffman & mask)
                fputs("1\0", fout);
            else
                fputs("0\0", fout);

            mask = mask >> 1;
        }
        fputs(", \0", fout);

        ++huffman;
    }
    fputc('\n', fout);

    fputs("};\n\0", fout);
    fputs("\n", fout);

    
    fputs("#endif\n\0", fout);
    fclose(fout);
    return 0;
}