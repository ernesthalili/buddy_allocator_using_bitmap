CCOPTS=-Wall -g -std=gnu99
VGOPTS=--leak-check=full --show-leak-kinds=all  --track-origins=yes  --verbose 
LIBS=-lm
CC=gcc


test:
	$(CC) $(CCOPTS) -o test Main.c bit_map.* Buddy_Allocator.* pool_allocator.* bit_map_tree.* $(LIBS)
	./test
	
test_vg:
	$(CC) $(CCOPTS) -o test_vg Main.c bit_map.*  Buddy_Allocator.* pool_allocator.* bit_map_tree.* $(LIBS)
	valgrind $(VGOPTS)--log-file=bud-valgrind-out.txt \./test_vg

	
clean_test:
	rm test
	rm test_vg    

