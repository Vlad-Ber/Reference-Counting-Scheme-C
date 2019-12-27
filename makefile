C_COMPILER	= gcc
C_OPTIONS	= -g -Wall -pedantic -std=c11
VALGRIND	= valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
CUNIT_LINK	= -lcunit
COVERAGE       = -fprofile-arcs -ftest-coverage --coverage
PROFILE        = -pg

run: proj/refmem.c
	$(C_COMPILER) -g -Wall proj/example.c -o memory
	./memory

valgrind: run
	$(VALGRIND) ./memory

test: proj/test.c proj/refmem.h proj/hash_table.c proj/list_linked.h proj/common.h proj/iterator.h
	$(C_COMPILER) $(C_OPTIONS) proj/test.c -o tests $(CUNIT_LINK)
	./tests

testValgrind: test
	$(VALGRIND) ./tests

coverage: test
	$(C_COMPILER) $(C_OPTIONS) $(COVERAGE) proj/test.c $(CUNIT_LINK) -o memory
	./memory
	gcov -b test.c

profiling: test
	$(C_COMPILER) $(C_OPTIONS) $(PROFILE) proj/test.c -o prof_output $(CUNIT_LINK)
