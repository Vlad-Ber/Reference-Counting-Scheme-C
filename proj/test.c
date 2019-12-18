#include "CUnit/Basic.h"
#include <stdbool.h>
#include "hash_table.h"
#include "list_linked.h"
#include "refmem.c"
#include "iterator.h"


struct cell1
{
  struct cell *cell;

  char *string;
  int k;
  int s;
  size_t mitocondria;
  int i;
  char *string2;
};

struct cell2
{
  struct cell2 *cell;
  char *string;
  int k;
};

//Destructor funktioner för cell1 och cell2
void cell_destructor1(obj *c){release(((struct cell1 *) c)->cell);}
void cell_destructor2(obj *c){release(((struct cell2 *) c)->cell);}

void test_retain()
{
  struct cell1 *c1 = allocate(sizeof(struct cell1), cell_destructor1);
  struct cell2 *c2 = allocate(sizeof(struct cell2), cell_destructor2);
  objectInfo_t *c1_Info = ((obj *) c1 - sizeof(objectInfo_t));
  objectInfo_t *c2_Info = ((obj *) c2 - sizeof(objectInfo_t));

  retain(c1);
  CU_ASSERT_TRUE(c1_Info->rf == 1);
  CU_ASSERT_TRUE(c2_Info->rf == 0);
  retain(c2);
 
}
void test_release()
{

}
void test_rc()
{

}

void test_allocate()
{

}

void test_allocate_array()
{

}

void test_deallocate()
{

}

void test_set_cascade_limit()
{

}

void test_get_cascade_limit()
{

}

void test_cleanup()
{

}

void test_shutdown()
{

}




int main(int argc, char *argv[])
{

  CU_initialize_registry();
  CU_pSuite unitTests =CU_add_suite("Enhetstester för refmem", NULL, NULL);
  
  CU_add_test(unitTests, "retain            unitTest",test_retain );
  CU_add_test(unitTests, "release           unitTest",test_release );
  CU_add_test(unitTests, "rc                unitTest",test_rc );
  CU_add_test(unitTests, "allocate          unitTest",test_allocate );
  CU_add_test(unitTests, "allocate_array    unitTest",test_allocate_array );
  CU_add_test(unitTests, "deallocate        unitTest",test_deallocate );
  CU_add_test(unitTests, "set_cascade_limit unitTest",test_set_cascade_limit );
  CU_add_test(unitTests, "get_cascade_limit unitTest",test_get_cascade_limit );
  CU_add_test(unitTests, "cleanup           unitTest",test_cleanup );
  CU_add_test(unitTests, "shutDown          unitTest",test_shutdown );
  
  CU_basic_run_tests();
  CU_cleanup_registry();

  return 0;
}
 
