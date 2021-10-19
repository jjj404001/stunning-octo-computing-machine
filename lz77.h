/*
Implementation of lz77 encoding algorith.
*/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct _lznode
{
    uint64_t Length;
    uint8_t  Literal; // Byte size literal
    uint64_t Distance;

    struct _lznode* Next;
} LzNode;

typedef struct _lzLinkedList
{
    uint64_t SizeInByte;
    LzNode* Head;
} LzLinkedlist;

LzLinkedlist EncodeLZ77(const void* _src, const size_t _size)
{
    LzLinkedlist linked_list;
    if(_size < 1)
        return linked_list; // NULL

    uint8_t* byte_src = (uint8_t*)_src;

    //const size_t window_count = 0x200;
    //const size_t view_count   = 0x200;
    const size_t window_count = 0x06;
    const size_t view_count   = 0x04;
    const size_t buffer_count = window_count + view_count;

    const size_t window_size = window_count * sizeof(uint8_t);
    const size_t view_size   = view_count   * sizeof(uint8_t);
    const size_t buffer_size = window_size + view_size;
    
    uint8_t* buffer = (uint8_t*)malloc(window_size + view_size);
    uint8_t* window = &buffer[0];
    uint8_t* view   = &buffer[window_count]; 

    uint64_t src_index    = 0;
    
    uint64_t total_size = 0;


    
    linked_list.Head = (LzNode*)malloc(sizeof(LzNode));

    LzNode* current = linked_list.Head;

    uint64_t window_start = window_count -1;
    uint64_t window_end   = 0;

    // Fill view buffer with inputs.
    {
        size_t curr_view_count = view_count;
        // First LDD is always be 0,0,view[0]
        current->Length   = 0;
        current->Distance = 0;
        current->Literal  = byte_src[0];

        ++total_size;

        uint64_t curr_count = 1;

        memcpy(view, &byte_src[1], view_size); // start from 1st data, since 0st data is gonna stored in window[0]
        window[window_start] = current->Literal;
        
        // first iteration to fill up the window buffer
        while(curr_count < window_count && curr_count < _size)
        {
            uint64_t i = window_start;

            while(i < window_count)
            {
                if(window[i] == view[0])
                    break;
                i++;
            }

            if (i == window_count) // if i is same with current window size, no p found inside window buffer.
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length = 0;
                new->Distance = 0;
                new->Literal  = view[0];

                current->Next = new;
                current = new;
            }
            else
            {
                LzNode* new = (LzNode*)malloc(sizeof(LzNode));
                new->Length   = 0;
                new->Distance = window_count - i;

                uint64_t max_it = curr_count;
                if(curr_count > curr_view_count) // TODO: use MIN here.
                    max_it = curr_view_count;

                uint64_t j = 0;
                while(max_it && window[i++] == view[j++])
                {
                    --max_it;
                    ++new->Length;
                }
                    

                if(!max_it)
                {
                    if(new->Length >= view_count)
                    {
                        new->Length = view_count-1;
                        new->Literal = view[new->Length];
                    }
                    else // else if literal still sit inside of the buffer
                    {
                        new->Literal = view[new->Length];
                    }
                }
                else
                    new->Literal = view[new->Length];

                current->Next = new;
                current = new;
            }


            
            
            if(window_start >= current->Length +1)
            {
                window_start -= current->Length +1;
                curr_count += current->Length +1;
                total_size += current->Length +1;
            }
            else
            {
                curr_count += window_start;
                total_size += window_start;
                window_start = 0;
            }
                
            
            memcpy(&window[window_start], &byte_src[src_index], window_count - window_start + curr_view_count);
        }

        total_size = curr_count;
    }

    uint64_t min_count = view_count;
    if(min_count > window_count) 
        min_count = window_count;

     
    while(total_size < _size)
    {
        uint64_t i = window_start;
        while(i < window_count)
        {
            if(window[i] == view[0])
                break;
            i++;
        }

        if (i == window_count) // if i is same with current window size, no p found inside window buffer.
        {
            LzNode* new = (LzNode*)malloc(sizeof(LzNode));
            new->Length = 0;
            new->Distance = 0;
            new->Literal  = view[0];

            current->Next = new;
            current = new;
        }
        else
        {
            LzNode* new = (LzNode*)malloc(sizeof(LzNode));
            new->Length   = 0;
            new->Distance = window_count - i;

            uint64_t max_it = min_count;

            uint64_t j = 0;
            while(max_it-- && window[i++] == view[j++])
                ++new->Length;

            // If src_index goes out of the src.
            if(total_size + new->Length + 1>= _size) // +1 for literal.
            {
                uint64_t diff = _size - total_size;
                new->Length  = diff - 1;
                new->Literal = view[new->Length];

                current->Next = new;
                current = new; 
                break; // break here to reduce memcpy
            }
            else
                new->Literal = view[new->Length];

            current->Next = new;
            current = new;
        }
        total_size += current->Length +1;
        src_index  += current->Length +1;
        memcpy(buffer, &byte_src[src_index], buffer_size);
    }
    current->Next = NULL;

    free((void*)buffer);

    linked_list.SizeInByte = _size;

    return linked_list;
}

uint8_t* DecodeLZ77(LzLinkedlist _linked_list)
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
}