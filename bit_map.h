#pragma once
#include "pool_allocator.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


typedef enum Status{
    ALLOCATED = 0x01,
    FREE = 0x00
}Status;

typedef enum OUT_MODE{
    STDOUT = 0,  //stdout
    F_WRITE = 1, //fprintf w
    F_CONCAT = 2 //fprintf a
} OUT_MODE;

typedef struct BitMap{
    uint8_t* Buf; //Bitmap will be stored here
    uint8_t* end_Buf;
    int buffer_size;
    int num_bits;
    int allocated_bits;
} BitMap;

int BitMap_getBytes(int bits);

BitMap* BitMap_init(PoolAllocator* p_alloc, int buf_size, uint8_t* buffer);

void BitMap_setBit(BitMap *bit_map, int bit_num, Status status);

uint8_t BitMap_bit( BitMap *bit_map, int bit_num);

void Bitmap_print(BitMap *bit_map, OUT_MODE out_mode);
















