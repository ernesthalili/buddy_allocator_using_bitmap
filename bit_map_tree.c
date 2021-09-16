#include "bit_map_tree.h"


int tree_get_idx(Buddy_item *bud){
    if(bud) return bud->idx;
    else return -1;
}

int tree_level(int idx){
    int ret = floor(log2(idx)); //2^level=node_idx => floor(log_2(node_idx)) = level
    if(ret>=0) return ret;
    else return 0;
}

int tree_first_node_level(int idx){
    return 0x0001<<tree_level(idx);
}

int tree_first_free_node_level(BitMap_tree* tree,int level){
    if(level == 0) {
        if(BitMap_bit(tree->BitMap,0) ==ALLOCATED)return -1;
        else return 0;
    }
    for(int i=pow(2, level+1)-1;i>0;--i){
        if(BitMap_bit(tree->BitMap, i)==FREE) return i+1;
    }
    return -1;
}

int tree_node_level_offset(int idx){
    return tree_first_node_level(tree_level(idx))-idx;
}

int tree_getbuddy(int idx){
    return (idx & 0x0001)?(idx-1):(idx);
}

int tree_getparent(int idx){
    return (uint16_t)idx/2;
}

int tree_buddiesOnLevel(BitMap_tree *tree, int level){
    if(level == 0){
        if (BitMap_bit(tree->BitMap, 0)) return 1;
        else return 0;
    }
    
    if(level == 1){
        if (BitMap_bit(tree->BitMap, 1)==ALLOCATED && BitMap_bit(tree->BitMap, 2)==ALLOCATED) return 2;
        if (BitMap_bit(tree->BitMap, 1)==ALLOCATED || BitMap_bit(tree->BitMap, 2)==ALLOCATED) return 1;
        else return 0;
    }
    
    int start_idx = pow(2, level);
    int end_idx = pow(2, level+1);
    int ret = 0;
    
    for(int i = start_idx; i<end_idx; i++){
        if(BitMap_bit(tree->BitMap, i)==ALLOCATED)ret++;
    }
    
    return ret;
}

void tree_print(BitMap_tree *tree, OUT_MODE out_mode){
    if(out_mode==F_WRITE){
        FILE* f = fopen("OUT/Logs/bitmap_tree.txt", "w");
        fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
        fprintf(f, "Tree Metadata:\n");
        fprintf(f, "%d bits\t%d bytes\n", tree->BitMap->num_bits, tree->BitMap->buffer_size);
        fprintf(f, "%p start\t%p end\n", tree->BitMap->Buf, tree->BitMap->end_Buf);
        for (int i = 0; i < tree->levels; i++){
            fprintf(f,"Level %d: %d buddies\t",i, tree_buddiesOnLevel(tree, i));
            fprintf(f,"First_free: %d\n",tree_first_free_node_level(tree, i));  
        }
        fprintf(f, "\n");
        fprintf(f, "Bitmap STATUS:\n");
        for (int i = 0; i < tree->levels; i++){
		    for (int j = 0; j < pow(2,i);j++){
			    fprintf(f, "%x",BitMap_bit(tree->BitMap,pow(2,i)+j));
		    }
		fprintf(f,"\n");
	    }
        fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
        fclose(f);
    }
    if(out_mode==STDOUT){
        fprintf(stdout, "\n----------------------------------------------------------------------------------------------\n");
        fprintf(stdout,"Tree Metadata:\n"); 
        fprintf(stdout,"%d bits\t%d bytes\n",tree->BitMap->num_bits, tree->BitMap->buffer_size);
        fprintf(stdout, "%p start\t%p end\n", tree->BitMap->Buf, tree->BitMap->end_Buf);
        for (int i = 0; i < tree->levels; i++){
            fprintf(stdout,"Level %d: %d buddies\t",i, tree_buddiesOnLevel(tree, i));
            fprintf(stdout,"First_free: %d\n",tree_first_free_node_level(tree, i));
        }
        fprintf(stdout,"\n");
        fprintf(stdout,"Bitmap STATUS:\n");
        for (int i = 0; i < tree->levels; i++){
		    for (int j = 0; j < pow(2,i);j++){
			    fprintf(stdout, "%x",BitMap_bit(tree->BitMap,pow(2,i)+j));
		    }
		fprintf(stdout,"\n");
	    }
        fprintf(stdout,"\n----------------------------------------------------------------------------------------------\n");
    }
    if(out_mode==F_CONCAT){
        FILE* f = fopen("OUT/Logs/bitmap_tree.txt", "a");
        fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
        fprintf(f, "Tree Metadata:\n");
        fprintf(f, "%d bits\t%d bytes\n",  tree->BitMap->num_bits, tree->BitMap->buffer_size);
        fprintf(f, "%p start\t%p end\n", tree->BitMap->Buf, tree->BitMap->end_Buf);
        for (int i = 0; i < tree->levels; i++){
            fprintf(f,"Level %d: %d buddies\t",i, tree_buddiesOnLevel(tree, i));
            fprintf(f,"First_free: %d\n",tree_first_free_node_level(tree, i));
        }
        fprintf(f,"\n");
        fprintf(f, "Bitmap STATUS:\n");
        for (int i = 0; i < tree->levels; i++){
		    for (int j = 0; j < pow(2,i);j++){
			    fprintf(f,"%x",BitMap_bit(tree->BitMap,(pow(2,i)+j)-1));
		    }
		fprintf(f,"\n");
	    }
        fprintf(f, "\n----------------------------------------------------------------------------------------------\n");
        fclose(f);
    }
}

int tree_nodes(int levels){
    return (pow(2, levels+1))-1;
}

int tree_leafs(int levels){
    return (pow(2, levels));
}

BitMap_tree* BitMap_tree_init(BitMap_tree *tree, BitMap* b, int levels){
    tree->BitMap = b;
    tree->leaf_num = tree_leafs(levels);
    tree->levels = levels;
    tree->total_nodes = tree_nodes(levels);
    return tree;
}


















