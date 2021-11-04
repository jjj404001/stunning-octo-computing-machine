#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define HEAPPARENT(x) (((x) - 1) / 2)
#define HEAPLEFT(x) ((2 * (x)) + 1)
#define HEAPRIGHT(x) ((2 * (x)) + 2)

typedef struct _heap
{
    uint64_t Size;
    uint64_t MaxSize;
    void* Data;
    size_t ElementSize;
} Heap; 

void HeapInit(Heap* _heap, uint64_t _maxSize, size_t _elementSize)
{
    memset(_heap, 0, sizeof(Heap));
    _heap->MaxSize = _maxSize;
    _heap->ElementSize = _elementSize;
    _heap->Data = malloc(_elementSize * _maxSize);
    //_heap->Size = 1; // Start from 1 since the index start from 1.

    memset(_heap->Data, 0, _elementSize * _maxSize);
}

void UpHeapByte(Heap* _heap, uint64_t _index);

void HeapSwap(Heap* _heap, uint64_t _index0, uint64_t _index1)
{
    uint8_t* temp = (uint8_t*)malloc(_heap->ElementSize);
    memcpy(temp, ((uint8_t*)_heap->Data + (_heap->ElementSize * _index0)), _heap->ElementSize);

    void* data0 = ((uint8_t*)_heap->Data + (_heap->ElementSize * _index0));
    void* data1 = ((uint8_t*)_heap->Data + (_heap->ElementSize * _index1));

    memcpy(data0, data1, _heap->ElementSize);
    memcpy(data1, temp, _heap->ElementSize);
}

void HeapInsertByte(Heap* _heap, uint8_t _new)
{
    if(_heap->Size >= _heap->MaxSize)
    {
        // grow size or just stop here
        // stop here for now
    }

    uint8_t* Data = (uint8_t*)_heap->Data;
    Data[++_heap->Size] = _new;
    UpHeapByte(_heap, _heap->Size);
}

void UpHeapByte(Heap* _heap, uint64_t _index)
{
    uint8_t* Data = (uint8_t*)_heap->Data;


    while(_index > 1 && Data[_index/2] > Data[_index]) 
    {
        // Swap if it is not a root node, and parent is smaller than its child node.
        HeapSwap(_heap, _index/2, _index);

        _index /= 2;
    }
}

void HeapDeleteByte(Heap* _heap, uint64_t _index)
{
    // Heap is also priority queue,
    // binary tree is just one way to implement the heap.
    if(_heap->Size == 0)
        return;

    HeapSwap(_heap, 1, _heap->Size-1);
    _heap->Size -= 1;

    // Down heap
    uint64_t left_i  = HEAPLEFT(_index);
    uint64_t right_i = HEAPRIGHT(_index);
    uint64_t curr_i  = 1;

    void* left  = ((uint8_t*)_heap->Data + (_heap->ElementSize * left_i));
    void* right = ((uint8_t*)_heap->Data + (_heap->ElementSize * right_i));

    void* curr  = ((uint8_t*)_heap->Data + _heap->ElementSize);
    
    
    while(curr_i < _heap->Size)
    {
        uint64_t left_i  = HEAPLEFT(curr_i);
        uint64_t right_i = HEAPRIGHT(curr_i);

        if(*(uint64_t*)left > *(uint64_t*)curr) // skip if same
        {
            HeapSwap(_heap, left_i, curr_i);
            curr_i = left_i;
        }
        else if (*(uint64_t*)right > *(uint64_t*)curr)
        {
            HeapSwap(_heap, right_i, curr_i);
            curr_i = right_i;
        }
        else
        {
            break;
        }
    }
    
}