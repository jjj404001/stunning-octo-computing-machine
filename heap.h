#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct _heap
{
    uint64_t Size;
    uint64_t MaxSize;
    void* Data;
    size_t ElementSize;
} Heap; 

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
        // grow size
    }

    uint8_t* Data = (uint8_t*)_heap->Data;
    Data[_heap->Size] = _new;
    UpHeapByte(_heap, _heap->Size);
    _heap->Size += 1;
}

void UpHeapByte(Heap* _heap, uint64_t _index)
{
    if(_index >= _heap->Size)
        return;

    uint8_t* Data = (uint8_t*)_heap->Data;


    while(_index > 1 && Data[_index/2] < Data[_index]) 
    {
        // Swap if it is not a root node, and parent is smaller than its child node.
        HeapSwap(_heap, _index/2, _index);

        _index /= 2;
    }
}

void HeapDelete(Heap* _heap)
{
    // Heap is also priority queue,
    // binary tree is just one way to implement the heap.
    if(_heap->Size == 0)
        return;

    HeapSwap(_heap, 1, _heap->Size-1);

    void* data = ((uint8_t*)_heap->Data + (_heap->ElementSize * (_heap->Size -1)));
    memset(data, 0, _heap->ElementSize);

    //
}