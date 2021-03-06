/*
Implementation of lz77 encoding algorith.
*/
#include "lz77.h"

LzLinkedlist EncodeLZ77(const void* _src, const size_t _size)
{
    LzLinkedlist linked_list;
    if(_size < 1)
        return linked_list; // NULL

    uint8_t* byte_src = (uint8_t*)_src;

    const size_t window_count = MAX_D;
    const size_t view_count   = MAX_D;
    //const size_t window_count = 0x06;
    //const size_t view_count   = 0x04;
    const size_t extra_count  = LZMIN(window_count, view_count);
    const size_t buffer_count = window_count + view_count + (extra_count * 2);

    const size_t window_size = window_count * sizeof(uint8_t);
    const size_t view_size   = view_count   * sizeof(uint8_t);
    const size_t buffer_size = buffer_count * sizeof(uint8_t);

    
    //uint8_t* buffer = (uint8_t*)malloc(buffer_size);
    uint8_t* buffer = byte_src;
    uint8_t* window = &buffer[0];
    uint8_t* view   = &buffer[0]; 
    uint64_t p = 0;
    uint64_t src_index    = 0;
    
    uint64_t total_size = 0;

    linked_list.SizeInByte = 0;
    linked_list.NodeCount  = 0;
    linked_list.Head = (LzNode*)malloc(sizeof(LzNode));
    linked_list.DCounter = (uint16_t*)malloc(sizeof(uint16_t) * MAX_D);
    memset(linked_list.DCounter, 0, sizeof(uint16_t) * MAX_D);
    linked_list.LCounter = (uint16_t**)malloc(sizeof(uint16_t*) * 256);
    
    {
        int i = 0;
        for(i = 0; i < 256; i++)
        {
            linked_list.LCounter[i] = (uint16_t*)malloc(sizeof(uint16_t) * MAX_D);
            memset(&linked_list.LCounter[i], 0, sizeof(uint16_t) * MAX_D);
        }
    }


    LzNode* current = linked_list.Head;

    uint64_t view_start = 0;
    uint64_t view_end   = view_count - 1;
    uint64_t window_start = 0;
    uint64_t window_end   = 0;

    int bool = 0;
    while(total_size < _size)
    {
        uint64_t i = window_start;
        uint64_t j = view_start;
        uint64_t inc = 0;
        while(i < window_end)
        {
            if(window[i] == view[j])
                break;
            ++i;
        }
        // Not found
        if(i == window_end)
        {
            LzNode* new = (LzNode*)malloc(sizeof(LzNode));
            new->Length   = 0;
            new->Distance = 0;
            new->Literal  = view[view_start];
            ++linked_list.NodeCount;

            ++linked_list.DCounter[new->Distance];

            current->Next = new; 
            current = new;
        }
        else // found
        {
            LzNode* new = (LzNode*)malloc(sizeof(LzNode));
            new->Distance = p - i; // i > p??
            new->Length = 0;
            ++linked_list.NodeCount;
            
            while(new->Length + i < window_end)
            {
                ++new->Length;
                ++j;
                if(window[i + new->Length] != view[j])
                    break;
            }

            if(view_end > new->Length + view_start)
            {
                new->Literal = view[new->Length + view_start];
            }
            else
            {
                new->Length  = view_count-1; 
                new->Literal = view[new->Length + view_start];
            }

            ++linked_list.DCounter[new->Distance];

            current->Next = new; 
            current = new;
        }
        inc += current->Length +1;

        total_size += inc;

        // Treat out of boundary issue 
        window_end += inc;
        if(bool == 1)
        {
            window_start += inc;
        }
        else if(window_end >= window_count) // TODO: can be optimized
        {
            window_start += window_end - window_count;
            bool = 1;
        }
            

        view_start += inc;
        view_end += inc;

        p += inc;
    }


    // pull single slot
    linked_list.Head = linked_list.Head->Next;
    current->Next = NULL;
    //free((void*)buffer);
    linked_list.SizeInByte = _size;

    return linked_list;
}

uint8_t* DecodeFreeLZ77(LzLinkedlist _linked_list)
{
    LzNode* node = _linked_list.Head;
    uint8_t* result = (uint8_t*)malloc(_linked_list.SizeInByte +1);

    uint64_t src_index = 0;
    while(node)
    {
        if(node->Length > 0)
        {
            memcpy(&result[src_index], &result[src_index - node->Distance], node->Length);
            result[src_index + node->Length] = node->Literal;
            src_index += node->Length + 1;
        }
        else
            result[src_index++] = node->Literal;

        LzNode* next = node->Next;
        free(node);
        node = next;
    }

    if(_linked_list.DCounter)
        free(_linked_list.DCounter);
        
    if(_linked_list.LCounter)
        free(_linked_list.LCounter);

    return result;
}

void FreeLZ77(LzLinkedlist _linked_list)
{
    LzNode* node = _linked_list.Head;

    while(node)
    {
        LzNode* next = node->Next;
        free(node);
        node = next;
    }

    if(_linked_list.DCounter)
        free(_linked_list.DCounter);

    if(_linked_list.LCounter)
        free(_linked_list.LCounter);
}

int SaveLZ77(LzLinkedlist _linked_list, const char* _str)
{
    FILE* out = fopen(_str, "w");
    
    LzNode* node = _linked_list.Head;
    while(node)
    {
        LzNode* next = node->Next;
        fwrite (next , sizeof(LzNode), 1, out);

        node = next;
    }
    fclose(out);
    
    return 1;
}

int SaveFreeLZ77(LzLinkedlist _linked_list, const char* _str)
{
    FILE* out = fopen(_str, "w");
    
    LzNode* node = _linked_list.Head;
    while(node)
    {
        LzNode* next = node->Next;
        fwrite (next , sizeof(LzNode), 1, out);

        free(node);
        node = next;
    }
    fclose(out);

    if(_linked_list.DCounter)
        free(_linked_list.DCounter);
        
    if(_linked_list.LCounter)
        free(_linked_list.LCounter);
    
    return 1;
}