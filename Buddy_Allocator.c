#include "Buddy_Allocator.h"


int BuddyAllocator_calcSize(int num_levels){
		
    int num_items=(1<<(num_levels+1));
    int list_alloc_size = (sizeof(Buddy_item)+sizeof(int))*num_items;
    return list_alloc_size;
}


Buddy_item* BuddyAllocator_createItem(BuddyAllocator* b_alloc, int idx){
    
    Buddy_item* item = PoolAllocator_getBlock(b_alloc->p_alloc);
    
    item->idx=idx;
    item->level = tree_level(idx);
    item->tree = b_alloc->tree;
    item->memory = b_alloc->memory + ((idx - (1 << tree_level(idx)))<< (b_alloc->num_levels - item->level)) *b_alloc->min_bucket_size;
    item->size =(1 << (b_alloc->num_levels - item->level)) * b_alloc->min_bucket_size;
    item->parent_idx = tree_getparent(idx);
    item->buddy_idx = tree_getbuddy(idx);

    BitMap_setBit(b_alloc->tree->mappa, idx, ALLOCATED);
    
    return item;
}


void BuddyAllocator_destroyItem(BuddyAllocator* b_alloc, Buddy_item* item){
    PoolAllocatorResult res = PoolAllocator_releaseBlock(b_alloc->p_alloc, item);
    assert(res==Success);
    BitMap_setBit(b_alloc->tree->mappa, item->idx, FREE);
}


void BuddyAllocator_init(BitMap_tree* tree, BuddyAllocator* b_alloc, PoolAllocator* p_alloc, uint8_t* bm_buffer, uint8_t* memory, int buffer_size, int num_levels ){
    
    b_alloc->num_levels=num_levels;
    b_alloc->memory=memory;
    b_alloc->tree = tree;
    b_alloc->min_bucket_size; //Valore non inizzializata
    b_alloc->buffer_size = buffer_size;
    b_alloc->num_items = 1<<(num_levels+1);
    assert (num_levels<MAX_LEVELS);
    
    // controllo se la memoria e sufficiente per le strutture interne
    assert (buffer_size>=BuddyAllocator_calcSize(num_levels));

    int list_items=1<<(num_levels+1); // massimo numero di allocazioni 
    int list_alloc_size=(sizeof(Buddy_item)+sizeof(int))*list_items;

    
    uint8_t *list_start= (uint8_t*)memory;
    
    Buddy_item* item0 =BuddyAllocator_createItem(b_alloc, 0); 
}

void BuddyAllocator_initSingleBuffer(BuddyAllocator* alloc, PoolAllocator* p_alloc, uint8_t* allocator_mem, BitMap_tree* tree, int allocator_mem_size, int num_levels){
   
    // Controlla se il buffer e la memoria sono inizializzati corretamente
    assert(alloc);
    assert(allocator_mem_size!=0 && allocator_mem);

    alloc->p_alloc = p_alloc;

    const int total_nodes = pow(2, num_levels + 1) - 1;
    const int internal_mem_required = (sizeof(Buddy_item)+sizeof(int))* total_nodes;
    const int user_mem = allocator_mem_size-internal_mem_required;
    const int min_bucket_size = (user_mem>>(num_levels));
    const int user_mem_required = min_bucket_size * tree_nodes(num_levels);
    const int leafs = (user_mem / min_bucket_size);
    const int total_mem_required = internal_mem_required + user_mem;
    
    uint8_t* list_start = allocator_mem; 
    PoolAllocatorResult init_result = PoolAllocator_init(p_alloc, sizeof(Buddy_item), total_nodes, list_start, internal_mem_required);

    
    alloc->num_levels = num_levels;
    alloc->min_bucket_size = min_bucket_size;
    alloc->memory = &allocator_mem[0] + internal_mem_required;
    alloc->buffer_size = allocator_mem_size;
    alloc->num_items = tree_nodes(num_levels);
    alloc->num_levels = num_levels;
    alloc->tree = tree;

    // Il primo elemento 
    Buddy_item *item = BuddyAllocator_createItem(alloc, 0);
}

Buddy_item* BuddyAllocator_getBuddy(BuddyAllocator* b_alloc, int level){

    if(level<0) return 0;
    assert(level<=b_alloc->num_levels);
    
    // Controlla se il buddy esiste, altrimenti passa al livello precedente 
    if(!tree_buddiesOnLevel(b_alloc->tree, level)){
        Buddy_item* parent = BuddyAllocator_getBuddy(b_alloc, level-1);
        if(!parent) return NULL;
    }
    if(tree_buddiesOnLevel(b_alloc->tree, level)){
        printf("Buddies on level:\n");
        int idx = tree_first_free_node_level(b_alloc->tree, level);
        return (Buddy_item*)b_alloc->memory+(idx-(1<<tree_level(idx)))*b_alloc->min_bucket_size;
    }
    return(0);
}

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, Buddy_item* item){
    Buddy_item* parent = (Buddy_item*) alloc->memory[item->parent_idx];
    Buddy_item* buddy = (Buddy_item*) alloc->memory[item->buddy_idx];
    
    BitMap_setBit(alloc->tree->mappa, item->idx, FREE);
    if(tree_getparent(item->idx)==item->idx)
        return;
    if(BitMap_bit(alloc->tree->mappa, tree_getbuddy(item->idx)))
        return;

    BuddyAllocator_destroyItem(alloc, item);
    BuddyAllocator_destroyItem(alloc, buddy);
    BuddyAllocator_destroyItem(alloc, parent);
    
}


void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size){
    
    int mem_size = (1<<alloc->num_levels)*alloc->min_bucket_size;
    int level = tree_level(mem_size/(size+8));
    if(level>alloc->num_levels) level = alloc->num_levels;
    
    Buddy_item* buddy = BuddyAllocator_getBuddy(alloc, level);

    if(!buddy) return 0;

    Buddy_item** target = (Buddy_item**)(buddy->memory);
    *target = buddy;
    return buddy->memory+8;
}

void BuddyAllocator_free(BuddyAllocator* alloc, void* mem){

    char* p = (char*) mem; p-=8;
    Buddy_item* buddy_ptr = (Buddy_item*)p;
    Buddy_item* buddy = buddy_ptr;
    BuddyAllocator_releaseBuddy(alloc, buddy);
}


void BuddyAllocator_printMetadata(BuddyAllocator* b_alloc){
        printf("\n----------------------------------------------------------------------------------------------\n");
        printf("\tBitmap Address: %p\n", b_alloc->tree->mappa);
        printf("\tBuddy Allocator levels: %d\n", b_alloc->num_levels);
        printf("\tBuddy Allocator items: %d\n", b_alloc->num_items);
        printf("\tBuddy Allocator min bucket size: %d\n", b_alloc->min_bucket_size);
        printf("\tMem alloc size: %d\n", b_alloc->buffer_size);
        printf("\tMem address start: %p\n", b_alloc->memory);
        printf("\tMem address end: %p\n", (b_alloc->memory)+b_alloc->buffer_size);
        Bitmap_print(b_alloc->tree->mappa);
        printf( "\n----------------------------------------------------------------------------------------------\n");
}














