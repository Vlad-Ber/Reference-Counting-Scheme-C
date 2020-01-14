#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "linked_list.h"
#include "iterator.h"
#include <CUnit/Basic.h>

struct list
{
    int size;
    link_t *head;       // points to first entry
    link_t *tail;    // points to last entry
};

struct link
{
    int val;
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

static bool int_comp(elem_t x, elem_t y) //////equalfunc for int compare
{
    return (x.int_val == y.int_val);
}

static bool is_greater_than(elem_t value, elem_t value_ignored, void *extra)
{
    elem_t *value2 = extra;
    return value.int_val > value2->int_val;
}


static bool string_comp(elem_t x, elem_t y) //////equalfunc for str compare
{
    return (x.str_val == y.str_val);
}


// Skapa lista först, insert sen, skapa iterator, ta sönder båda

void test_iter_has_next1(void)     //tests next on existing
{
    ioopm_list_t *list = ioopm_linked_list_create(int_comp);
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(11));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_has_next2(void)     // tests next on non existing
{
    ioopm_list_t *list = ioopm_linked_list_create(int_comp);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_next1(void)  //next on exist
{
    ioopm_list_t *list = ioopm_linked_list_create(int_comp);
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(11));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    CU_ASSERT_TRUE(ioopm_iterator_next(iter).success);
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_next2(void) //next on non exist
{
    ioopm_list_t *list = ioopm_linked_list_create(int_comp);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    CU_ASSERT_FALSE(ioopm_iterator_next(iter).success);
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_current1(void) //////////////////////
{
    ioopm_list_t *list = ioopm_linked_list_create(int_comp);
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(11));
    ioopm_linked_list_insert(list, 2, int_elem(12));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).value.int_val, 11);
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_current2(void)  ///////////////////////
{
    ioopm_list_t *list = ioopm_linked_list_create(int_comp);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    CU_ASSERT_FALSE(ioopm_iterator_current(iter).success);
    ioopm_iterator_destroy(iter);
    ioopm_linked_list_destroy(list);
}

void test_iter_reset(void) // insert on non exist index
{
    ioopm_list_t *list = ioopm_linked_list_create(int_comp);
    ioopm_linked_list_insert(list, 0, int_elem(10));
    ioopm_linked_list_insert(list, 1, int_elem(11));
    ioopm_linked_list_insert(list, 2, int_elem(12));
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).value.int_val, 11);
    ioopm_iterator_next(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).value.int_val, 12);
    ioopm_iterator_reset(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).value.int_val, 10);
    ioopm_iterator_destroy(iter);
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
        (NULL == CU_add_test(test_suite1, "test 1 (HasNext1)", test_iter_has_next1))    ||
        (NULL == CU_add_test(test_suite1, "test 2 (HasNext2)", test_iter_has_next2))    ||
        (NULL == CU_add_test(test_suite1, "test 3 (Next1)", test_iter_next1))           ||
        (NULL == CU_add_test(test_suite1, "test 4 (Next2)", test_iter_next2))           ||
        (NULL == CU_add_test(test_suite1, "test 5 (Current1)", test_iter_current1))     ||
        (NULL == CU_add_test(test_suite1, "test 6 (Current2)", test_iter_current2)      ||
         (NULL == CU_add_test(test_suite1, "test 7 (Reset)", test_iter_reset))



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
