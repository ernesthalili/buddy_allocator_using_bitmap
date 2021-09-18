#include <stdint.h>
#include <math.h>
#include <stdio.h>

#include "bit_map_tree.h"

// Dato un elemento ritorna il suo indice, -1 se non inizializzato
int tree_get_idx(Buddy_item *bud)
{
    if(bud) return bud->idx;
    else return -1;
}

// Dato un indice, ritorna il livello nell'albero in cui esso si trova
int tree_level(int idx)
{
    int ret = floor(log2(idx)); //2^level=node_idx => floor(log_2(node_idx)) = level
    if(ret>=0) return ret;
    else return 0;
}

// Dato un livello, ritorna il primo indice del livello
int tree_first_node_level(int idx)
{
    return 0x0001<<tree_level(idx);
}

// Dato un BitMap e un livello , ritorna l'indice che corrisponde al primo elemnto libero
int tree_first_free_node_level(BitMap_tree* tree,int level)
{
    if(level == 0) 
    {
        if(BitMap_bit(tree->mappa,0) ==ALLOCATED)return -1;
        else return 0;
    }
    
    for(int i=pow(2, level+1)-1;i>0;--i)
    {
        if(BitMap_bit(tree->mappa, i)==FREE) return i+1;
    }
    
    return -1;
}

// Calcola l'offset dell'indice 
int tree_node_level_offset(int idx)
{
    return tree_first_node_level(tree_level(idx))-idx;
}

// Ritorna l'indice del buddy
int tree_getbuddy(int idx)
{
    return (idx & 0x0001)?(idx-1):(idx);
}

// Ritorna l'indice del genitore
int tree_getparent(int idx)
{
    return (uint16_t)idx/2;
}

// controlla i bit settati del livello
int tree_buddiesOnLevel(BitMap_tree *tree, int level)
{
    if(level == 0)
    {
        if (BitMap_bit(tree->mappa, 0)) return 1;
        else return 0;
    }
    
    if(level == 1)
    {
        if (BitMap_bit(tree->mappa, 1)==ALLOCATED && BitMap_bit(tree->mappa, 2)==ALLOCATED) return 2;
        if (BitMap_bit(tree->mappa, 1)==ALLOCATED || BitMap_bit(tree->mappa, 2)==ALLOCATED) return 1;
        else return 0;
    }
    
    int start_idx = pow(2, level);
    int end_idx = pow(2, level+1);
    int ret = 0;
    
    for(int i = start_idx; i<end_idx; i++)
    {
        if(BitMap_bit(tree->mappa, i)==ALLOCATED)ret++;
    }
    
    return ret;
}

void tree_print(BitMap_tree *tree)
{
        printf("\n----------------------------------------------------------------------------------------------\n");
        printf("Tree Metadata:\n");
        printf("%d bits\t%d bytes\n",  tree->mappa->num_bits, tree->mappa->buffer_size);
        printf("%p start\t%p end\n", tree->mappa->Buf, tree->mappa->end_Buf);
        for (int i = 0; i < tree->levels; i++)
        {
            printf("Level %d: %d buddies\t",i, tree_buddiesOnLevel(tree, i));
            printf("First_free: %d\n",tree_first_free_node_level(tree, i));
        }
        printf("\n");
        printf("Bitmap STATUS:\n");
        for (int i = 0; i < tree->levels; i++)
        {
		    for (int j = 0; j < pow(2,i);j++)
		    {
			    printf("%x",BitMap_bit(tree->mappa,(pow(2,i)+j)-1));
		    }
		printf("\n");
	    }
        printf("\n----------------------------------------------------------------------------------------------\n");
       
    
}

// Calcola i nodi di un livello
int tree_nodes(int levels)
{
    return (pow(2, levels+1))-1;
}

// Calcola le foglie di un certo livello
int tree_leafs(int levels)
{
    return (pow(2, levels));
}

// Funzione che inizializza l'albero di BitMap
BitMap_tree* BitMap_tree_init(BitMap_tree *tree, BitMap* b, int levels)
{
    tree->mappa = b;
    tree->leaf_num = tree_leafs(levels);
    tree->levels = levels;
    tree->total_nodes = tree_nodes(levels);
    return tree;
}


















