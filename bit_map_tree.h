#include "bit_map.h"

#include <stdint.h>
#include <math.h>
#include <stdio.h>

typedef struct Bitmap_tree{
    BitMap* BitMap;
    int levels;
    int total_nodes;
    int leaf_num;
}BitMap_tree;


typedef struct Buddy_item{
    char* memory;
    BitMap_tree* tree;
    int idx;
    int parent_idx;
    int buddy_idx;
    int size;
    int level;
}Buddy_item;


int tree_get_idx(Buddy_item *bud);

int tree_level(int idx);

int tree_first_node_level(int idx);

int tree_first_free_node_level(BitMap_tree* tree,int level);

int tree_node_level_offset(int idx);

int tree_getbuddy(int idx);

int tree_getparent(int idx);

int tree_buddiesOnLevel(BitMap_tree *tree, int level);

void tree_print(BitMap_tree *tree, OUT_MODE out);

int tree_nodes(int levels);

int tree_leafs(int levels);

BitMap_tree* BitMap_tree_init(BitMap_tree *tree, BitMap* b, int levels);














