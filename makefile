run:proj/refmem.c
	gcc -g -Wall proj/example.c -o memory
	./memory

valgrind: run
	valgrind --leak-check=full --track-origins=yes ./memory

test: proj/test.c proj/refmem.h proj/hash_table.c proj/list_linked.h proj/common.h proj/iterator.h
	gcc -g -Wall proj/test.c -o tests -lcunit
	./tests
