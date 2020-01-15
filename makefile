C_COMPILER	= gcc
C_OPTIONS	= -g -Wall -std=c11
VALGRIND	= valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
CUNIT_LINK	= -lcunit
COVERAGE       = -fprofile-arcs -ftest-coverage --coverage
PROFILE        = -pg


run: src/refmem.c
	$(C_COMPILER) -g -Wall test/example.c -o memory
	./memory


valgrind: run
	$(VALGRIND) ./memory

test: test/test.c src/refmem.h 
	$(C_COMPILER) $(C_OPTIONS) test/test.c -o tests $(CUNIT_LINK)
	./tests

testValgrind: test
	$(VALGRIND) ./tests

coverage: test
	$(C_COMPILER) $(C_OPTIONS) $(COVERAGE) test/test.c $(CUNIT_LINK) -o memory
	./memory
	gcov -b test.c


prof_org:
	gcc -pg -Wall demo/inlupp2_v2_original/test_warehouse.c demo/inlupp2_v2_original/user_interface.c demo/inlupp2_v2_original/business_logic.c 	demo/inlupp2_v2_original/utils.c demo/inlupp2_v2_original/hash_table.c demo/inlupp2_v2_original/linked_list.c demo/inlupp2_v2_original/iterator.h demo/inlupp2_v2_original/common.h -o prof_inlupp2_org -lcunit

	./prof_inlupp2_org
	gprof prof_inlupp2_org > analys_old.txt
	rm gmon.out

prof_v2:
	gcc -pg -Wall demo/inlupp2_v2/test_warehouse.c demo/inlupp2_v2/user_interface.c demo/inlupp2_v2/business_logic.c 	demo/inlupp2_v2/utils.c demo/inlupp2_v2/hash_table.c demo/inlupp2_v2/linked_list.c demo/inlupp2_v2/iterator.h demo/inlupp2_v2/common.h -o prof_inlupp2_v2 -lcunit

	./prof_inlupp2_v2
	gprof prof_inlupp2_v2 > analys.txt
	rm gmon.out

clean:
	rm -f *.o *.gch *.gcov *.info linked_list_tests hash_table_tests main business_logic user_interface utils linked_list_cov hash_table_cov



demo  = demo/inlupp2_v2/test_warehouse.c demo/inlupp2_v2/user_interface.c demo/inlupp2_v2/business_logic.c 	demo/inlupp2_v2/utils.c demo/inlupp2_v2/hash_table.c demo/inlupp2_v2/linked_list.c demo/inlupp2_v2/iterator.h demo/inlupp2_v2/common.h

src = $(wildcard src/*.c)
#demo = $(wildcard demo/inlupp2_v2/*.c)
test_files = $(wildcard test/*.c)

src_o = $(src:.c = .o)
demo_o = $(demo: .c = .o)
test_files_o = $(test_files: .c = .o) 


demo_build: $(demo_o) $(src_o)
	$(C_COMPILER) $(C_OPTIONS)  $(demo_o) -lcunit -o $@

demo_tests: demo_build
	valgrind --leak-check=full ./demo_build


# Compile and link warehouse(main), business_logic, user_interface, hash_table, linked_list & utils
compile_main: demo/inlupp2_v2/main.o demo/inlupp2_v2/user_interface.o demo/inlupp2_v2/business_logic.o demo/inlupp2_v2/utils.o demo/inlupp2_v2/hash_table.o demo/inlupp2_v2/linked_list.o src/refmem.o $(src_o)
	$(C_COMPILER) $(C_OPTIONS) demo/inlupp2_v2/main.o demo/inlupp2_v2/user_interface.o demo/inlupp2_v2/business_logic.o demo/inlupp2_v2/utils.o demo/inlupp2_v2/hash_table.o demo/inlupp2_v2/linked_list.o  -o main

main: compile_main
	valgrind --leak-check=full ./main

#run: compile
#	./warehouse
