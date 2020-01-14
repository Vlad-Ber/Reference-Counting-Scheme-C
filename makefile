C_COMPILER	= gcc
C_OPTIONS	= -g -Wall -std=c11
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


prof_compile_org:
	gcc -pg -Wall proj/inlupp2_v2_original/test_warehouse.c proj/inlupp2_v2_original/user_interface.c proj/inlupp2_v2_original/business_logic.c 	proj/inlupp2_v2_original/utils.c proj/inlupp2_v2_original/hash_table.c proj/inlupp2_v2_original/linked_list.c proj/inlupp2_v2_original/iterator.h proj/inlupp2_v2_original/common.h -o prof_inlupp2_org -lcunit
	
	./prof_inlupp2_org
	gprof prof_inlupp2_org > analys_old.txt

prof_compile_v2:
	gcc -pg -Wall proj/inlupp2_v2/test_warehouse.c proj/inlupp2_v2/user_interface.c proj/inlupp2_v2/business_logic.c 	proj/inlupp2_v2/utils.c proj/inlupp2_v2/hash_table.c proj/inlupp2_v2/linked_list.c proj/inlupp2_v2/iterator.h proj/inlupp2_v2/common.h -o prof_inlupp2_v2 -lcunit
	rm gmon.out
	./prof_inlupp2_org
	gprof prof_inlupp2_org > analys.txt


