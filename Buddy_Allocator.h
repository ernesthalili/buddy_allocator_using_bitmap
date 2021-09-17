#pragma once

#include "pool_allocator.h"
#include "bit_map_tree.h" 
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEVELS 16

typedef struct  BuddyAllocator{
    int num_levels;
    int buffer_size;
    int min_bucket_size;
    int num_items;
    BitMap_tree* tree;
    PoolAllocator *p_alloc;
    char* memory;
} BuddyAllocator;


int BuddyAllocator_calcSize(int num_levels);

Buddy_item* BuddyAllocator_createItem(BuddyAllocator* b_alloc, int idx);

void BuddyAllocator_destroyItem(BuddyAllocator* b_alloc, Buddy_item* item);

void BuddyAllocator_init( BitMap_tree* tree, BuddyAllocator* b_alloc, PoolAllocator* p_alloc, char* bm_buffer, char* memory, int buffer_size, int num_levels );
                    
void BuddyAllocator_initSingleBuffer(BuddyAllocator* alloc, PoolAllocator* p_alloc,  char* allocator_mem, BitMap_tree* tree, int allocator_mem_size, int num_levels);

Buddy_item* BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level);

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, Buddy_item* item);

void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);

void BuddyAllocator_printMetadata(BuddyAllocator* alloc, OUT_MODE out);

