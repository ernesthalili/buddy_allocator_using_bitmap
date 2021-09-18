#pragma once

#include <stdint.h>
#include <stdio.h>

typedef enum 
{
  Success=0x0,
  NotEnoughMemory=-1,
  UnalignedFree=-2,
  OutOfRange=-3,
  DoubleFree=-4
} PoolAllocatorResult;

typedef struct PoolAllocator
{ 
  uint8_t* buffer;     // memoria contigua
  int*  free_list;     // lista collegata di oggetti
  int buffer_size;     // dimensione del buffer

  int size;            // numero dei blocchi liberi
  int size_max;        // numero massimo di blocchi
  int item_size;       // dimensione di un blocco
  
  int first_idx;       // puntatore al primo bucket
  int bucket_size;     // dimensione bucket
} PoolAllocator;


PoolAllocatorResult PoolAllocator_init(PoolAllocator* allocator , int item_size, int num_items, uint8_t* memory_block, int memory_size);

void* PoolAllocator_getBlock(PoolAllocator* allocator);

PoolAllocatorResult PoolAllocator_releaseBlock(PoolAllocator* allocator, void* block);

const char* PoolAllocator_strerror(PoolAllocatorResult result);
