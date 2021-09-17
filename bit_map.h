#pragma once
#include "pool_allocator.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


// I possibili stati che possono avere i bit
typedef enum Status{
    ALLOCATED = 0x01,
    FREE = 0x00
}Status;


typedef struct BitMap{
    uint8_t* Buf; 
    uint8_t* end_Buf;
    int buffer_size;
    int num_bits;
    int allocated_bits;
} BitMap;

int BitMap_getBytes(int bits);

BitMap* BitMap_init(PoolAllocator* p_alloc, int buf_size, uint8_t* buffer);

void BitMap_setBit(BitMap *bit_map, int bit_num, Status status);

uint8_t BitMap_bit( BitMap *bit_map, int bit_num);

void Bitmap_print(BitMap *bit_map);
















