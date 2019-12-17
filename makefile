run:proj/refmem.c
	gcc -g -Wall proj/example.c -o memory
	./memory

valgrind: run
	valgrind --leak-check=full --track-origins=yes ./memory
