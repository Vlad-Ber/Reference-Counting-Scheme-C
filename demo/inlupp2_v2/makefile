clean:
	rm -f *.o *.gch *.gcov *.info linked_list_tests hash_table_tests main business_logic user_interface utils linked_list_cov hash_table_cov

# Variables
# --------------------------------------------------
C_COMPILER		= gcc
C_OPTIONS		= -Wall -g

# For CUnit
CUNIT_LINK		= -lcunit

# Program specific

LINKED_LIST_DEPS	= linked_list.h iterator.h common.h 	
LINKED_LIST_OBJS 	= linked_list.o
LINKED_LIST_TOBJS	= linked_list.o linked_list_tests.o

HASH_TABLE_DEPS		= hash_table.h $(LINKED_LIST_DEPS)
HASH_TABLE_OBJS		= hash_table.o linked_list.o
HASH_TABLE_TOBJS	= hash_table.o linked_list.o hash_table_tests.o

WAREHOUSE_DEPS		= utils.h user_interface.h business_logic.h main.c $(HASH_TABLE_DEPS) 
WAREHOUSE_OBJS		= $(HASH_TABLE_DEPS) user_interface.o business_logic.o utils.o hash_table.o linked_list.o
WAREHOUSE_TOBJS 	= hash_table.o linked_list.o test_warehouse.o user_interface.o business_logic.o utils.o $(HASH_TABLE_DEPS)

PROF_OBJS		= refmem.c business_logic.o hash_table.o linked_list.o

FREQ_DEFAULT_ARG	= 16k-words.txt

# --------------------------------------------------
# GENERAL

# Compile generic src file
%.o:	%.c
	$(C_COMPILER) $(C_OPTIONS) $? -c

# Compile hash_table object
hash_table: main.o business_logic.h user_interface.h hash_table.h linked_list.h utils.h 
	@echo Compiling Warehouse

# Compile linked_list object
linked_list: linked_list.o linked_list.h iterator.h common.h
	@echo Compiling linked list

# Compile and link warehouse(main), business_logic, user_interface, hash_table, linked_list & utils
compile: main.o user_interface.o business_logic.o utils.o hash_table.o linked_list.o $(HASH_TABLE_DEPS)
	$(C_COMPILER) $(C_OPTIONS) main.o user_interface.o business_logic.o utils.o hash_table.o linked_list.o -o warehouse

run: compile
	./warehouse

# --------------------------------------------------
# TESTS

test_warehouse: $(WAREHOUSE_TOBJS) iterator.h common.h
	$(C_COMPILER) $(C_OPTIONS) $(WAREHOUSE_TOBJS) -o $@ $(CUNIT_LINK)

test: test_warehouse
	./test_warehouse

testcompile: test_warehouse.o hash_table.o linked_list.o user_interface.o business_logic.o utils.o $(HASH_TABLE_DEPS)
	$(C_COMPILER) $(C_OPTIONS) test_warehouse.o hash_table.o linked_list.o user_interface.o business_logic.o utils.o -o testwarehouse -lcunit

testrun: testcompile
	./testwarehouse

memtest: testcompile
	valgrind --leak-check=full ./testwarehouse

#--------------------------------------------------
#COVERAGE

cov_warehouse:
	$(C_COMPILER) $(C_OPTIONS) --coverage test_warehouse.c user_interface.c business_logic.c utils.c hash_table.c linked_list.c iterator.h common.h -o warehouse_cov $(CUNIT_LINK)
	./warehouse_cov
	gcov -abcfu business_logic.h test_warehouse.c


# --------------------------------------------------
# VALGRIND

memcheck: compile
	valgrind --leak-check=full ./warehouse < test_inputs.txt

mem: test_warehouse
	valgrind --leak-check=yes ./test_warehouse


#---------
# Profiling

prof_objs: $(WAREHOUSE_TOBJS) iterator.h common.h
	$(C_COMPILER) $(C_OPTIONS) $(WAREHOUSE_TOBJS) -pg -o $@ $(CUNIT_LINK)

prof: prof_objs
	rm -f gmon.out
	./prof_objs
	gprof gmon.out prof_objs > profile.txt
	more profile.txt


