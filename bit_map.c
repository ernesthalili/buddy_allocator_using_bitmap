#include "bit_map.h"

// Ritorna il numero dei bytes per memorizzare i bits
int BitMap_getBytes(int bits)
{
  return bits/8 + ((bits%8)!=0);
}

// Inizializza il BitMap usando un array esterno
BitMap* BitMap_init(PoolAllocator* p_alloc, int buf_size, uint8_t* buffer)
{  
    PoolAllocatorResult res =  PoolAllocator_init(p_alloc, sizeof(BitMap), 1, buffer, buf_size);
    assert(res == 0);
    
    BitMap* bit_map = (BitMap*) PoolAllocator_getBlock(p_alloc);
    assert(buffer!=NULL);
    bit_map->Buf = buffer;
    bit_map->num_bits = buf_size;
    bit_map->buffer_size = BitMap_getBytes(buf_size);
    bit_map->end_Buf = buffer+buf_size-1;
    printf(" buf_size :%d\n",buf_size);
    
    for(int i = 0; i<buf_size; i++)
    {
		BitMap_setBit(bit_map, i, FREE);
	}
	
    return bit_map;
}
// Al bit_num della BitMap setta lo stato che puo avere valore {0,1}
void BitMap_setBit(BitMap *bit_map, int bit_num, Status status)
{
    int page = bit_num>>3;
    assert(page<bit_map->buffer_size);
    int offset =  bit_num %8;
    if(status==ALLOCATED) 
        bit_map->Buf[page] |= (1U<<offset);
    if(status==FREE)
        bit_map->Buf[page] &= ~(1U<<offset);
}

// controlla lo stato del bit che si trova al indice bit_num
uint8_t BitMap_bit(BitMap *bit_map, int bit_num)
{
    int page = bit_num>>3;
    assert(page<bit_map->buffer_size);
    int offset =  bit_num %8;
    
    return (bit_map->Buf[page] & (1<<offset))!=0; 
}

void Bitmap_print(BitMap *bit_map)
{
    if(bit_map->Buf != NULL){
            printf("\n----------------------------------------------------------------------------------------------\n");
            printf("Bitmap Metadata:\n");
            printf("%d bits\t%d bytes\n",  bit_map->num_bits, bit_map->buffer_size);
            printf("%p start\t%p end\n", bit_map->Buf, bit_map->end_Buf);
            printf("Bitmap STATUS:\n");
            for(int i=0; i<bit_map->num_bits; i++)
            {
                printf("%x", BitMap_bit(bit_map, i));
            }
            printf("\n----------------------------------------------------------------------------------------------\n");
    }
}










