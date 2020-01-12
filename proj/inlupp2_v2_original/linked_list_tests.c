#define _XOPEN_SOURCE 700
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "common.h"
#include "linked_list.h"


struct list
{
  size_t size;
  link_t *head;       // points to first entry
  link_t *tail;    // points to last entry
};

struct link
{
  elem_t val;
  link_t *next;     //points to next link
};

int init_suite(void)
  
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

bool int_comp(elem_t x, elem_t y) //////equalfunc for int compare
{
  return (x.int_val == y.int_val);
}

bool is_greater_than(elem_t value, elem_t value_ignored, void *extra)
{
  elem_t *value2 = extra;
  return value.int_val > value2->int_val;  
}


bool string_comp(elem_t x, elem_t y) //////equalfunc for str compare
{
  return (x.str_val == y.str_val);
}



void test_list_append1(void)     //append 1 link
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_append(list, int_elem(1));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0).value.int_val, 1)
        ioopm_linked_list_destroy(list);
}

void test_list_append2(void)     // append on more links
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
   ioopm_linked_list_insert(list, 1, int_elem(1));
   ioopm_linked_list_insert(list, 2, int_elem(1));
  ioopm_linked_list_append(list, int_elem(10));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 3).value.int_val, 10);
  ioopm_linked_list_destroy(list);
}

void test_list_prepend1(void)  //prepend 1 link empty list
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 0).value.int_val, 1)
        ioopm_linked_list_destroy(list);
}

void test_list_prepend2(void) //prepend 1 link
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  ioopm_linked_list_insert(list, 2, int_elem(3));
  ioopm_linked_list_prepend(list, int_elem(4));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 1).value.int_val, 1)
  ioopm_linked_list_destroy(list);
}

void test_list_insert1(void) // insert in empty list
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(1));
  CU_ASSERT_TRUE(ioopm_linked_list_get(list, 1).success)
  ioopm_linked_list_destroy(list);
}

void test_list_insert2(void) // insert on same index
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 0, int_elem(1));
  CU_ASSERT_TRUE(ioopm_linked_list_get(list, 1).success)
  ioopm_linked_list_destroy(list);
}

void test_list_insert3(void) // insert on non exist index
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 3, int_elem(1));
  CU_ASSERT_FALSE(ioopm_linked_list_get(list, 3).success)
    ioopm_linked_list_destroy(list);
}

void test_list_insert4(void) // TODO, STRING?
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 0, int_elem(1));
  CU_ASSERT_TRUE(ioopm_linked_list_get(list, 1).success)
     ioopm_linked_list_destroy(list);
}

void test_list_remove1(void) // remove one elem
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_remove(list, 0);
  CU_ASSERT_FALSE(ioopm_linked_list_get(list, 1).success);
    ioopm_linked_list_destroy(list);
}

void test_list_remove2(void) // remove on empty list
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_remove(list, 0);
  CU_ASSERT_FALSE(ioopm_linked_list_get(list, 0).success);
    ioopm_linked_list_destroy(list);
}

void test_list_get1(void) // Retrieves value in specified index
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  CU_ASSERT_EQUAL(ioopm_linked_list_get(list, 1).value.int_val, 2);
    ioopm_linked_list_destroy(list);
}
 
void test_list_get2(void) // Retrieves value in specified index
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get(list, 0).value.int_val, 2);
    ioopm_linked_list_destroy(list);
}
 
void test_list_contains1(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list, int_elem(0), int_comp));
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list, int_elem(10), int_comp));
    ioopm_linked_list_destroy(list);
}
 
void test_list_contains2(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list, int_elem(0), int_comp));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(2), int_comp));
    ioopm_linked_list_destroy(list);
}
 
void test_list_size1(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 2);
  CU_ASSERT_NOT_EQUAL(ioopm_linked_list_size(list), 1);
    ioopm_linked_list_destroy(list);
}
 
void test_list_size2(void)
{
   ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  CU_ASSERT_EQUAL(ioopm_linked_list_size(list), 0);
      ioopm_linked_list_destroy(list);
}
 
void test_list_is_empty1(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}
 
void test_list_is_empty2(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  CU_ASSERT_FALSE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}
 
void test_list_clear1(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  ioopm_linked_list_clear(list);
  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}
 
void test_list_clear2(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_clear(list);
  CU_ASSERT_TRUE(ioopm_linked_list_is_empty(list));
    ioopm_linked_list_destroy(list);
}

void test_list_all1(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  int value = 0;
  CU_ASSERT_TRUE(ioopm_linked_list_all(list, is_greater_than, &value));
    ioopm_linked_list_destroy(list);
}
 
void test_list_all2(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  int value = 1;
  CU_ASSERT_FALSE(ioopm_linked_list_all(list, is_greater_than, &value));
    ioopm_linked_list_destroy(list);
}
 
void test_list_any1(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  int value = 1;
  CU_ASSERT_TRUE(ioopm_linked_list_any(list, is_greater_than, &value));
    ioopm_linked_list_destroy(list);
}
 
void test_list_any2(void)
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(1));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  int value = 3;
  CU_ASSERT_FALSE(ioopm_linked_list_any(list, is_greater_than, &value));
    ioopm_linked_list_destroy(list);
}


static void value_add(elem_t value_ignored, elem_t *value, void *extra)
{
  elem_t *value2 = extra;
  value->int_val += *(int*)value2; //type cast to int and deref
}

void test_list_apply_all1(void) //FEL EFTER ELEM_T
{
  ioopm_list_t *list = ioopm_linked_list_create(int_comp);
  ioopm_linked_list_insert(list, 0, int_elem(2));
  ioopm_linked_list_insert(list, 1, int_elem(2));
  ioopm_linked_list_insert(list, 2, int_elem(2));
  int value = 2;
  ioopm_linked_apply_to_all(list, value_add, &value);
  CU_ASSERT_EQUAL((ioopm_linked_list_get(list, 0).value.int_val), 4);
  CU_ASSERT_NOT_EQUAL((ioopm_linked_list_get(list, 1).value.int_val), 2);
  CU_ASSERT_EQUAL((ioopm_linked_list_get(list, 2).value.int_val), 4);
    ioopm_linked_list_destroy(list);
}

int main()
{
  CU_pSuite test_suite1 = NULL;
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  test_suite1 = CU_add_suite("Test Suite 1", init_suite, clean_suite);
  if (NULL == test_suite1)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if (
      (NULL == CU_add_test(test_suite1, "test 1 (Append1)", test_list_append1))     ||
      (NULL == CU_add_test(test_suite1, "test 2 (Append2)", test_list_append2))     ||
      (NULL == CU_add_test(test_suite1, "test 3 (Prepend1)", test_list_prepend1))   ||
      (NULL == CU_add_test(test_suite1, "test 4 (prepend2)", test_list_prepend2))   ||
      (NULL == CU_add_test(test_suite1, "test 5 (insert1)", test_list_insert1))     ||
      (NULL == CU_add_test(test_suite1, "test 7 (insert2)", test_list_insert2))     ||
      (NULL == CU_add_test(test_suite1, "test 8 (insert3)", test_list_insert3)      ||
      (NULL == CU_add_test(test_suite1, "test 9 (insert4)", test_list_insert4))     ||    
      (NULL == CU_add_test(test_suite1, "test 10 (remove1)", test_list_remove1))      ||
      (NULL == CU_add_test(test_suite1, "test 11 (remove2)", test_list_remove2))      ||
      (NULL == CU_add_test(test_suite1, "test 12 (get1)", test_list_get1))          ||
      (NULL == CU_add_test(test_suite1, "test 13 (get2)", test_list_get2))          ||
      (NULL == CU_add_test(test_suite1, "test 14 (contains1)", test_list_contains1))||
      (NULL == CU_add_test(test_suite1, "test 15 (contains2)", test_list_contains2))||
      (NULL == CU_add_test(test_suite1, "test 16 (size1)", test_list_size1))        ||
      (NULL == CU_add_test(test_suite1, "test 17 (size2)", test_list_size2))        ||
      (NULL == CU_add_test(test_suite1, "test 18 (empty1)", test_list_is_empty1))   ||
      (NULL == CU_add_test(test_suite1, "test 19 (empty2)", test_list_is_empty2))   ||
      (NULL == CU_add_test(test_suite1, "test 20 (clear1)", test_list_clear1))      ||
      (NULL == CU_add_test(test_suite1, "test 21 (clear2)", test_list_clear2))      ||
      (NULL == CU_add_test(test_suite1, "test 22 (all1)", test_list_all1))          ||
      (NULL == CU_add_test(test_suite1, "test 23 (all2)", test_list_all2))          ||
      (NULL == CU_add_test(test_suite1, "test 24 (any1)", test_list_any1))          ||
      (NULL == CU_add_test(test_suite1, "test 25 (any2)", test_list_any2))          ||
      (NULL == CU_add_test(test_suite1, "test 26 (all1)", test_list_apply_all1))

       ))
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
