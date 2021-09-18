#include "pool_allocator.h"

static const int NullIdx=-1;
static const int DetachedIdx=-2;

static const char* PoolAllocator_strerrors[]=
  {"Success",
   "NotEnoughMemory",
   "UnalignedFree",
   "OutOfRange",
   "DoubleFree",
   0
  };
  
const char* PoolAllocator_strerror(PoolAllocatorResult result) 
{
  return PoolAllocator_strerrors[-result];
}

PoolAllocatorResult PoolAllocator_init(PoolAllocator* a, int item_size, int num_items, uint8_t* memory_block, int memory_size) 
{
  // Si controlla se la memoria e' sufficiente per il bookkeeping
  int requested_size= num_items*(item_size+sizeof(int));
  if (memory_size<requested_size)
    return NotEnoughMemory;

  a->item_size=item_size;
  a->size=num_items;
  a->buffer_size=item_size*num_items;
  a->size_max = num_items;
  
  a->buffer=memory_block; // la prima parte si usa come memoria
  a->free_list= (int*)(memory_block+item_size*num_items); // la parte finale per il bookkeeping

 
  for (int i=0; i<a->size-1; ++i)
  {
    a->free_list[i]=i+1;
  }
  // settiamo a NULL l'ultimo elemento della lista
  a->free_list[a->size-1] = NullIdx;
  a->first_idx=0;
  
  return Success;
}

void* PoolAllocator_getBlock(PoolAllocator* a) 
{
  if (a->first_idx==-1)
      return 0;

  int detached_idx = a->first_idx;
  
  // scorre l'elemento
  a->first_idx=a->free_list[a->first_idx];
  --a->size;
  
  a->free_list[detached_idx]=DetachedIdx;
  
  // Adesso la referenca e' quella successiva
  uint8_t* block_address=a->buffer+(detached_idx*a->item_size);
  
  return block_address;
}

PoolAllocatorResult PoolAllocator_releaseBlock(PoolAllocator* a, void* block_)
{
  uint8_t* block=(uint8_t*) block_;
  int offset=block - a->buffer;

  // Controllo di corretteza 
  if (offset%a->item_size)
    return UnalignedFree;

  int idx=offset/a->item_size;

  // controllo per vedere se ci troviamo dentro il buffer
  if (idx<0 || idx>=a->size_max)
    return OutOfRange;

  // Controllo se il blocco e; gia libero
  if (a->free_list[idx]!=DetachedIdx)
    return DoubleFree;

  // si aggiunge alla testa
  a->free_list[idx]=a->first_idx;
  a->first_idx=idx;
  ++a->size;
  
  return Success;
}




