#include "CUnit/Basic.h"
#include <stdbool.h>
#include "hash_table.h"
#include "list_linked.h"
#include "refmem.c"
#include "iterator.h"
#include <stdio.h>
#include <string.h>

typedef struct cell1 cell1_t;
typedef struct cell2 cell2_t;


struct cell1
{
    cell1_t *cell;

    char *string;
    int k;
    int s;
    size_t mitocondria;
    int i;
    char *string2;
};

struct cell2
{
    cell2_t *cell;
    char *string;
    int k;
};

//Destructor funktioner för cell1 och cell2
void cell_destructor1(obj *c)
{
    release(((cell1_t *) c)->cell);
}
void cell_destructor2(obj *c)
{
    release(((cell2_t *) c)->cell);
}

void test_retain()
{
    first_info = NULL;
    last_info = NULL;

    cell1_t *c1 = allocate(sizeof(cell1_t), cell_destructor1);
    cell2_t *c2 = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_Info = ((obj *) c1 - sizeof(objectInfo_t));
    objectInfo_t *c2_Info = ((obj *) c2 - sizeof(objectInfo_t));
    CU_ASSERT_TRUE(c1_Info->rf == 0);
    CU_ASSERT_TRUE(c2_Info->rf == 0);
    retain(c1);
    retain(c2);
    CU_ASSERT_TRUE(c1_Info->rf == 1);
    CU_ASSERT_TRUE(c2_Info->rf == 1);
    retain(c2);
    CU_ASSERT_TRUE(c1_Info->rf == 1);
    CU_ASSERT_TRUE(c2_Info->rf == 2);
    c2_Info->rf = 0;
    c1_Info->rf = 0;

    c1->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_nextInfo = ((obj *) c1->cell - sizeof(objectInfo_t));
    objectInfo_t *c2_nextInfo = ((obj *) c2->cell - sizeof(objectInfo_t));
    CU_ASSERT_TRUE(c1_nextInfo ->rf == 0);
    retain(c1->cell);
    CU_ASSERT_TRUE(c1_nextInfo ->rf == 1);
    CU_ASSERT_TRUE(c2_nextInfo ->rf == 0);
    retain(c2->cell);
    CU_ASSERT_TRUE(c2_nextInfo ->rf == 1);
    retain(c2->cell);
    c1_nextInfo->rf = 0;
    c2_nextInfo->rf = 0;


    c1->cell->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell->cell = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_nextNextInfo = ((obj *) c1->cell->cell - sizeof(objectInfo_t));
    objectInfo_t *c2_nextNextInfo = ((obj *) c2->cell->cell - sizeof(objectInfo_t));
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 0);
    retain(c1->cell->cell);
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 1);
    retain(c1->cell->cell);
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 2);
    retain(c1->cell->cell);
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 3);
    CU_ASSERT_TRUE(c2_nextNextInfo ->rf == 0);
    c1_nextNextInfo->rf = 0;
    c2_nextNextInfo->rf = 0;

    deallocate(c1);
    deallocate(c2);
}
void test_release()
{
    first_info = NULL;
    last_info = NULL;

    cell1_t *c1 = allocate(sizeof(cell1_t), cell_destructor1);
    cell2_t *c2 = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_Info = ((obj *) c1 - sizeof(objectInfo_t));
    objectInfo_t *c2_Info = ((obj *) c2 - sizeof(objectInfo_t));
    c1_Info->rf = 5;
    c2_Info->rf = 3;
    CU_ASSERT_TRUE(c1_Info ->rf == 5);
    CU_ASSERT_TRUE(c2_Info ->rf == 3);
    release(c1);
    release(c2);
    CU_ASSERT_TRUE(c1_Info ->rf == 4);
    CU_ASSERT_TRUE(c2_Info ->rf == 2);
    release(c1);
    release(c2);
    CU_ASSERT_TRUE(c1_Info ->rf == 3);
    CU_ASSERT_TRUE(c2_Info ->rf == 1);
    release(c1);
    c2_Info->rf = 0;
    c1_Info->rf = 0;

    c1->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_nextInfo = ((obj *) c1->cell - sizeof(objectInfo_t));
    objectInfo_t *c2_nextInfo = ((obj *) c2->cell - sizeof(objectInfo_t));

    c1_nextInfo->rf = 5;
    c2_nextInfo->rf = 3;
    CU_ASSERT_TRUE(c1_nextInfo ->rf == 5);
    CU_ASSERT_TRUE(c2_nextInfo ->rf == 3);
    release(c1->cell);
    release(c2->cell);
    CU_ASSERT_TRUE(c1_nextInfo ->rf == 4);
    CU_ASSERT_TRUE(c2_nextInfo ->rf == 2);
    release(c1->cell);
    release(c2->cell);
    CU_ASSERT_TRUE(c1_nextInfo ->rf == 3);
    CU_ASSERT_TRUE(c2_nextInfo ->rf == 1);
    release(c1->cell);
    CU_ASSERT_TRUE(c1_nextInfo ->rf == 2);

    c2_nextInfo->rf = 0;
    c1_nextInfo->rf = 0;



    c1->cell->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell->cell = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_nextNextInfo = ((obj *) c1->cell->cell - sizeof(objectInfo_t));
    objectInfo_t *c2_nextNextInfo = ((obj *) c2->cell->cell - sizeof(objectInfo_t));

    c1_nextNextInfo->rf = 5;
    c2_nextNextInfo->rf = 3;
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 5);
    CU_ASSERT_TRUE(c2_nextNextInfo ->rf == 3);
    release(c1->cell->cell);
    release(c2->cell->cell);
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 4);
    CU_ASSERT_TRUE(c2_nextNextInfo ->rf == 2);
    release(c1->cell->cell);
    release(c2->cell->cell);
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 3);
    CU_ASSERT_TRUE(c2_nextNextInfo ->rf == 1);
    release(c1->cell->cell);
    CU_ASSERT_TRUE(c1_nextNextInfo ->rf == 2);

    c2_nextNextInfo->rf = 0;
    c1_nextNextInfo->rf = 0;


    release(c1);
    release(c2);

}
void test_rc()
{
    first_info = NULL;
    last_info = NULL;

    cell1_t *c1 = allocate(sizeof(cell1_t), cell_destructor1);
    cell2_t *c2 = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_Info = ((obj *) c1 - sizeof(objectInfo_t));
    objectInfo_t *c2_Info = ((obj *) c2 - sizeof(objectInfo_t));
    c1_Info->rf = 5;
    c2_Info->rf = 3;
    CU_ASSERT_TRUE(rc(c1)== 5);
    CU_ASSERT_TRUE(rc(c2)== 3);
    c1_Info->rf = 10;
    c2_Info->rf = 5;
    CU_ASSERT_TRUE(rc(c1)== 10);
    CU_ASSERT_TRUE(rc(c2)== 5);
    c2_Info->rf = 0;
    c1_Info->rf = 0;

    c1->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_nextInfo = ((obj *) c1->cell - sizeof(objectInfo_t));
    objectInfo_t *c2_nextInfo = ((obj *) c2->cell - sizeof(objectInfo_t));
    c1_nextInfo->rf = 5;
    c2_nextInfo->rf = 3;
    CU_ASSERT_TRUE(rc(c1->cell) == 5);
    CU_ASSERT_TRUE(rc(c2->cell) == 3);
    c1_nextInfo->rf = 10;
    c2_nextInfo->rf = 5;
    CU_ASSERT_TRUE(rc(c1->cell)== 10);
    CU_ASSERT_TRUE(rc(c2->cell)== 5);
    c2_nextInfo->rf = 0;
    c1_nextInfo->rf = 0;
    CU_ASSERT_TRUE(rc(c1->cell)== 0);
    CU_ASSERT_TRUE(rc(c2->cell)== 0);

    c1->cell->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell->cell = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_nextNextInfo = ((obj *) c1->cell->cell - sizeof(objectInfo_t));
    objectInfo_t *c2_nextNextInfo = ((obj *) c2->cell->cell - sizeof(objectInfo_t));
    c1_nextNextInfo->rf = 5;
    c2_nextNextInfo->rf = 3;
    CU_ASSERT_TRUE(rc(c1->cell->cell) == 5);
    CU_ASSERT_TRUE(rc(c2->cell->cell) == 3);
    c1_nextNextInfo->rf = 10;
    c2_nextNextInfo->rf = 5;
    CU_ASSERT_TRUE(rc(c1->cell->cell) == 10);
    CU_ASSERT_TRUE(rc(c2->cell->cell) == 5);
    c2_nextNextInfo->rf = 0;
    c1_nextNextInfo->rf = 0;
    CU_ASSERT_TRUE(rc(c1->cell->cell) == 0);
    CU_ASSERT_TRUE(rc(c2->cell->cell) == 0);

    deallocate(c1);
    deallocate(c2);

}

void test_allocate()
{
    first_info = NULL;
    last_info = NULL;

    cell1_t *c1 = allocate(sizeof(cell1_t), cell_destructor1);
    cell2_t *c2 = allocate(sizeof(cell2_t), cell_destructor2);
    objectInfo_t *c1_Info = ((obj *) c1 - sizeof(objectInfo_t));
    objectInfo_t *c2_Info = ((obj *) c2 - sizeof(objectInfo_t));

    void *c1Pointer = c1;
    void *c2Pointer = c2;
    CU_ASSERT_TRUE(c1Pointer != c2Pointer);
    CU_ASSERT_TRUE(c1_Info != c2_Info);

    c1->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell = allocate(sizeof(cell2_t), cell_destructor2);

    void *c1NextPointer = c1->cell;
    void*c2NextPointer = c2->cell;
    CU_ASSERT_TRUE(c1NextPointer != c2NextPointer);

    c1->cell->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell->cell = allocate(sizeof(cell2_t), cell_destructor2);

    deallocate(c1);
    deallocate(c2);

}

void test_allocate_array()
{
    first_info = NULL;
    last_info = NULL;

    //IFALL VALGRIND INTE KLAGAR PÅ NÅGOT HÄR FUNKAR allocate_array
    cell1_t *c1 = allocate_array(5, sizeof(cell1_t), cell_destructor1);

    cell2_t *c2 = allocate_array(3,sizeof(cell2_t), cell_destructor2);
    cell1_t *c3 = allocate_array(7, sizeof(cell1_t), cell_destructor1);
    cell2_t *c4 = allocate_array(10,sizeof(cell2_t), cell_destructor2);


    //TODO: Fixa så att detta inte får valgrindfel
    cell1_t *secondCellc1 = c1+1;
    cell1_t *thirdCellc1 = c1+2;
    cell2_t *secondCellc2 = c2+1;
    cell1_t *fifthCellc3 = c3+4;
    cell2_t *tenthCellc4 = c4+9;

    secondCellc1 -> k = 5;
    thirdCellc1 -> k = 10;
    secondCellc2 -> k = 4;
    fifthCellc3 ->k = 7;
    tenthCellc4 ->k = 4;
    tenthCellc4 ->string = "hej";


    CU_ASSERT_TRUE(secondCellc1-> k == 5);
    CU_ASSERT_TRUE(thirdCellc1-> k == 10);
    CU_ASSERT_TRUE(secondCellc2-> k == 4);
    CU_ASSERT_TRUE(fifthCellc3-> k == 7);
    CU_ASSERT_TRUE(tenthCellc4-> k == 4);
    CU_ASSERT_EQUAL(strcmp(tenthCellc4-> string, "hej"), 0);

    retain(c1);
    shutdown();
}

void test_deallocate()
{
    //IFALL VALGRIND INTE KLAGAR PÅ NÅGOT HÄR FUNKAR DEALLOCATE
    first_info = NULL;
    last_info = NULL;

    cell1_t *c1 = allocate(sizeof(cell1_t), cell_destructor1);
    cell2_t *c2 = allocate(sizeof(cell2_t), cell_destructor2);

    c1->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell = allocate(sizeof(cell2_t), cell_destructor2);

    c1->cell->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell->cell = allocate(sizeof(cell2_t), cell_destructor2);

    deallocate(c1);
    deallocate(c2);
}

void test_set_cascade_limit()
{
    set_cascade_limit(17);
    CU_ASSERT_TRUE(17 == cascade_limit);

}

void test_get_cascade_limit()
{
    cascade_limit = 15;
    CU_ASSERT_TRUE(15 == get_cascade_limit());
}

void test_cleanup()
{
    first_info = NULL;
    last_info = NULL;
    cell1_t *c0 = allocate(sizeof(cell1_t), cell_destructor1);
    cell1_t *c1 = allocate(sizeof(cell1_t), cell_destructor1);
    cell2_t *c2 = allocate(sizeof(cell2_t), cell_destructor2);
    cell2_t *c3 = allocate(sizeof(cell2_t), cell_destructor2);

    c0->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c1->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell = allocate(sizeof(cell2_t), cell_destructor2);
    c3->cell = allocate(sizeof(cell2_t), cell_destructor2);


    retain(c1);
    retain(c2);

    //TODO: FIX ME!!! (Eller lägg till i deviations)
    //När vi kör cleanup kommer vi kalla free på c1->cell  då c1->cell==0.
    //Sen när vi kör release c1 kallas free på c1->cell igen.
    retain(c1->cell);
    retain(c2->cell);


    release(c1);
    release(c2);

    cleanup();
}

void test_shutdown()

{

    first_info = NULL;
    last_info = NULL;

    cell1_t *c0 = allocate(sizeof(cell1_t), cell_destructor1);
    cell1_t *c1 = allocate(sizeof(cell1_t), cell_destructor1);
    cell2_t *c2 = allocate(sizeof(cell2_t), cell_destructor2);
    cell2_t *c3 = allocate(sizeof(cell2_t), cell_destructor2);

    c0->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c1->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c2->cell = allocate(sizeof(cell2_t), cell_destructor2);
    c3->cell = allocate(sizeof(cell2_t), cell_destructor2);


    //Vi testar 3 olika scenarion.
    //c0 == 0, c0->cell = 0
    //c1 == 1, c1->cell = 1
    //c2 == 1, c2->cell = 0
    //c3 == 0, c3->cell = 1

    //retain(c1);
    //retain(c2);
    //retain(c1->cell);
    //retain(c3->cell);

    shutdown();
}

void test_cascade()
{
  printf("---cascade limit tests!!! \n");
    set_cascade_limit(2);
    CU_ASSERT_TRUE(2 == get_cascade_limit());

    cell1_t *c0 = allocate(sizeof(cell1_t), cell_destructor1);
    c0->cell = allocate(sizeof(cell1_t), cell_destructor1);
    c0->cell->cell = allocate(sizeof(cell1_t), cell_destructor1);
    //cell1_t *temp = c0->cell->cell;
    
    release(c0);
    //cell1_t *cl = get_last_cascade();
    //release(cl);
    //deallocate(temp);

    
    CU_ASSERT_TRUE(2 == get_cascade_limit());

    printf("---cascade limit tests FIN!!! \n");
      
}

void test_default_destructor()
{

    cell1_t *c0 = allocate(sizeof(cell1_t), NULL);
    cell2_t *c2 = allocate(sizeof(cell2_t), NULL);

    c0->cell = allocate(sizeof(cell1_t), NULL);
    c2->cell = allocate(sizeof(cell2_t), NULL);

    shutdown();


}


void test_empty_cleanup()
{
    cleanup();
}

void test_rc_with_null()
{
    rc(NULL);
}

int main(int argc, char *argv[])
{

    CU_initialize_registry();
    CU_pSuite unitTests =CU_add_suite("Enhetstester för refmem", NULL, NULL);

    CU_add_test(unitTests, "retain            unitTest",test_retain );
    CU_add_test(unitTests, "release           unitTest",test_release );
    CU_add_test(unitTests, "rc                unitTest",test_rc );



    CU_pSuite memTests = CU_add_suite("Enhetstester för funktioner där memleak ej ska finnas", NULL, NULL);

    CU_add_test(memTests, "allocate          unitTest",test_allocate );
    CU_add_test(memTests, "allocate_array    unitTest",test_allocate_array );
    CU_add_test(memTests, "deallocate        unitTest",test_deallocate );
    CU_add_test(memTests, "cleanup           unitTest",test_cleanup );
    CU_add_test(memTests, "shutdown          unitTest",test_shutdown );
    CU_add_test(memTests, "default destructorunitTest", test_default_destructor);

    CU_pSuite cascadeTests = CU_add_suite("Tests testing cascadelimit", NULL, NULL);
    CU_add_test(cascadeTests, "set_cascade_limit unitTest",test_set_cascade_limit );
    CU_add_test(cascadeTests, "get_cascade_limit unitTest",test_get_cascade_limit );
    CU_add_test(cascadeTests, "test cascadeLimit functionality", test_cascade);

    CU_pSuite edgeCases = CU_add_suite("Testing edge cases for gcov", NULL, NULL);
    CU_add_test(edgeCases, "empty Cleanup", test_empty_cleanup);
    CU_add_test(edgeCases, "rc with null", test_rc_with_null);



    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}

