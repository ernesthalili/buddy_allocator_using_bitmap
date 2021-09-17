CCOPTS=-Wall -g -std=gnu99
VGOPTS=--leak-check=full --show-leak-kinds=all  --track-origins=yes  --verbose 
LIBS=-lm
CC=gcc


main:
	$(CC) $(CCOPTS) -o main Main.c bit_map.* Buddy_Allocator.* pool_allocator.* bit_map_tree.* $(LIBS)
	./main
	
main_vg:
	$(CC) $(CCOPTS) -o main_vg Main.c bit_map.*  Buddy_Allocator.* pool_allocator.* bit_map_tree.* $(LIBS)
	valgrind $(VGOPTS)--log-file=main_valgrind_out.txt \./main_vg

test:
	$(CC) $(CCOPTS) -o test Tests/Bitmap_test.c bit_map.* Buddy_Allocator.* pool_allocator.* bit_map_tree.* $(LIBS)
	./test	

test_vg:
	$(CC) $(CCOPTS) -o test_vg Tests/Bitmap_test.c bit_map.*  Buddy_Allocator.* pool_allocator.* bit_map_tree.* $(LIBS)
	valgrind $(VGOPTS)--log-file=test_valgrind_out.txt \./test_vg

clean_main:
	rm main
	
clean_main_vg:
	rm main_vg 
	rm main_valgrind_out.txt  
	
clean_test:
	rm test

clean_test_vg:
	rm test_vg 
	rm test_valgrind_out.txt

