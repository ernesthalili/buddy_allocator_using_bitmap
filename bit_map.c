#include "bit_map.h"

int BitMap_getBytes(int bits){
  return bits/8 + (bits%8)!=0;
}


BitMap* BitMap_init(PoolAllocator* p_alloc, int buf_size, char* buffer){
    
    PoolAllocatorResult res =  PoolAllocator_init(p_alloc, sizeof(BitMap), 1, buffer, buf_size);
    
    BitMap* bit_map = (BitMap*) PoolAllocator_getBlock(p_alloc);
    assert(buffer!=NULL);
    bit_map->Buf = buffer;
    bit_map->num_bits = buf_size;
    bit_map->buffer_size = BitMap_getBytes(buf_size);
    bit_map->end_Buf = buffer+buf_size-1;
    
    for(int i = 0; i<buf_size; i++){
		BitMap_setBit(bit_map, i, FREE);
	}
	
    return bit_map;
}

void BitMap_setBit(BitMap *bit_map, int bit_num, Status status){
    int page = bit_num>>3;
    assert(page<bit_map->buffer_size);
    int offset =  bit_num %8;
    if(status==ALLOCATED) 
        bit_map->Buf[page] |= (1<<offset);
    if(status==FREE)
        bit_map->Buf[page] &= ~(1<<offset);
}


char BitMap_bit(BitMap *bit_map, int bit_num){
    int page = bit_num>>3;
    assert(page<bit_map->buffer_size);
    int offset =  bit_num %8;
    
    return (bit_map->Buf[page] & (1<<offset))!=0; 
}

void Bitmap_print(BitMap *bit_map, OUT_MODE out_mode){
    if(bit_map->Buf!=NULL){
        if(out_mode==F_WRITE){
            FILE* f = fopen("OUT/Logs/bitmap.txt", "w");
            fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
            fprintf(f, "Bitmap Metadata:\n");
            fprintf(f, "%d bits\t%d bytes\n", bit_map->num_bits, bit_map->buffer_size);
            fprintf(f, "%p start\t%p end\n", bit_map->Buf, bit_map->end_Buf);
            fprintf(f, "Bitmap STATUS:\n");
            for(int i=0; i<bit_map->num_bits; i++){
                fprintf(f, "%x", BitMap_bit(bit_map, i));  
            }
            fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
            fclose(f);
        }
        if(out_mode==STDOUT){
            fprintf(stdout, "\n----------------------------------------------------------------------------------------------\n");
            fprintf(stdout,"Bitmap Metadata:\n"); 
            fprintf(stdout,"%d bits\t%d bytes\n",bit_map->num_bits, bit_map->buffer_size);
            fprintf(stdout, "%p start\t%p end\n", bit_map->Buf, bit_map->end_Buf);
            fprintf(stdout,"Bitmap STATUS:\n");
            for(int i=0; i<bit_map->num_bits; i++){
                fprintf(stdout, "%x", BitMap_bit(bit_map, i));
            }
            fprintf(stdout,"\n----------------------------------------------------------------------------------------------\n");

        }
        if(out_mode==F_CONCAT){
            FILE* f = fopen("OUT/Logs/bitmap.txt", "a");
            fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
            fprintf(f, "Bitmap Metadata:\n");
            fprintf(f, "%d bits\t%d bytes\n",  bit_map->num_bits, bit_map->buffer_size);
            fprintf(f, "%p start\t%p end\n", bit_map->Buf, bit_map->end_Buf);
            fprintf(f, "Bitmap STATUS:\n");
            for(int i=0; i<bit_map->num_bits; i++){
                fprintf(f, "%x", BitMap_bit(bit_map, i));
            }
            fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
            fclose(f);
        }
    }
}










