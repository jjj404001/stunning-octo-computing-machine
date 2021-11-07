#ifndef _DEFLATE_H
#define _DEFLATE_H

#include <stdio.h>
#include "lz77.h"

void Deflate(const char* _out_path, LzLinkedlist* _lz)
{
    FILE* deflate_out = fopen("Encoded.glb", "wb");
    fwrite ("01" , 1, 3, deflate_out); // fixed
    fwrite ("0" , 1, 1, deflate_out); // non-endblock

    LzNode* node = _lz->Head;

    while(node)
    {
        
    }


    fclose(deflate_out);
}

#endif