main:	
C_COMPILER	= gcc
C_OPTIONS	= -Wall -pedantic -g
CUNIT_LINK	= -lcunit

%.o:	%.c
	$(C_COMPILER) $(C_OPTIONS) $? -c

comp: 	list_linked.o hash_table.o buisness_logic.o ../refmem.o
	$(C_COMPILER) $(C_OPTIONS) -c common.h iterator.h utils.c

test: 	tests.o list_linked.o hash_table.o ../refmem.o
	$(C_COMPILER) $(C_OPTIONS) --coverage common.h iterator.h utils.c $? -o test $(CUNIT_LINK)

test2:	hash_table.c list_linked.c utils.c tests.c 
	gcc -Wall -g --coverage --rc lcov_branch_coverage=1 hash_table.c list_linked.c utils.c tests.c -o test2 -lcunit

memtest2: test2
	valgrind --leak-check=full ./test2

lcov: memtest2
	lcov -c -d --rc lcov_branch_coverage=1 . -o test2.info

lcov_gen: test
	genhtml --rc genhtml_branch_coverage=1 . test2.info -o test2-lcov

ui:	UI.o hash_table.o buisness_logic.o utils.o list_linked.o frontend.o ../refmem.o
	$(C_COMPILER) $(C_OPTIONS) common.h $? -o main

run:	ui
	./main

run_ui:
	./main < text.txt

ui_memtest:
	valgrind --leak-check=full ./main < text.txt

memtest:
	valgrind --leak-check=full ./test

clean:
	rm -f *.o *.out *.gcno *.gcda test main
