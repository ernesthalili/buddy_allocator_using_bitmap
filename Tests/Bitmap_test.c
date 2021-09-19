#include "../bit_map.h"
#include "../bit_map_tree.h"
#include "../pool_allocator.h"
#include <stdio.h>


#define BUF_SIZE 512 // 512 bit Bitmap
#define MEM_SIZE (BUF_SIZE + sizeof(BitMap)) //Only 1 bitmap to save

uint8_t buffer[MEM_SIZE];
uint8_t memory[BUF_SIZE];

int main(int argc, char const *argv[]){

	PoolAllocator PAllocator;

	BitMap *b = BitMap_init(&PAllocator, BUF_SIZE, memory);

	for(int i = 0; i<BUF_SIZE; i++){
		BitMap_setBit(b, i, ALLOCATED);
	}
	Bitmap_print(b);

	for(int j = 0; j<BUF_SIZE; j++){
		BitMap_setBit(b, j, FREE);
	}
	Bitmap_print(b);

	BitMap_setBit(b, 3, ALLOCATED);
	Bitmap_print(b);
	
	BitMap_setBit(b, 3, FREE);
	Bitmap_print(b);

	BitMap_tree tree;
	BitMap_tree_init(&tree, b, 9);
	
	tree_print(&tree);

	for(int j = 0; j<BUF_SIZE; j++){
		if((j%2))BitMap_setBit(b, j, FREE);
		else BitMap_setBit(b, j, ALLOCATED);
	}
	
	tree_print(&tree);

	PoolAllocator_releaseBlock(&PAllocator, b);

	return 0;
}
