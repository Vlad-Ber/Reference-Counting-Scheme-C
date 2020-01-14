#include <CUnit/Basic.h>
#include <stdbool.h>
#include "common.h"
#include "buisness_logic.c"

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

void test_add_merch()
{
    ioopm_db_t *db = ioopm_database_create();
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 0);
    char *name = "a0";
    char *desc = "a0";
    int price = 0;
    ioopm_add_merch(db, strdup(name), strdup(desc), price);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 1);
    ioopm_add_merch(db, strdup(name), strdup(desc), price);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 1);
    char *name2 = "a1";
    char *desc2 = "a1";
    ioopm_add_merch(db, strdup(name2), strdup(desc2), price);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 2);
    shutdown();
    //ioopm_destroy_database(db);
}

void test_remove_merch()
{
    ioopm_db_t *db = ioopm_database_create();
    char *name1 = "a1";
    char *name2 = "a2";
    char *name3 = "a3";
    char *desc1 = "a1";
    char *desc2 = "a2";
    char *desc3 = "a3";
    int price = 0;
    ioopm_remove_merch(db, name1);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 0);
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_add_merch(db, strdup(name2), strdup(desc2), price);
    ioopm_add_merch(db, strdup(name3), strdup(desc3), price);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 3);

    ioopm_remove_merch(db, name1);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 2);

    ioopm_remove_merch(db, name2);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 1);
    ioopm_remove_merch(db, name2);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 1);
    ioopm_remove_merch(db, name3);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 0);

    ioopm_destroy_database(db);
}

void test_edit_merch()
{
    ioopm_db_t *db = ioopm_database_create();
    char *name1 = "a1";
    char *name2 = "a2";
    char *name3 = "a3";
    char *desc1 = "a1";
    char *desc2 = "a2";
    char *desc3 = "a3";
    int price = 0;
    ioopm_edit_merch(db, name2, name1, desc1, price);
    CU_ASSERT(ioopm_hash_table_size(db->merch_db) == 0);
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_add_merch(db, strdup(name2), strdup(desc2), price);
    ioopm_add_merch(db, strdup(name3), strdup(desc3), price);
    ioopm_edit_merch(db, name1, name2, desc1, price);
    ioopm_remove_merch(db, name1);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 2);
    char *new_name = "b1";
    char *new_desc = "b1";
    ioopm_edit_merch(db, name2, strdup(new_name), strdup(new_desc), price);
    ioopm_remove_merch(db, new_name);
    CU_ASSERT_TRUE(ioopm_hash_table_size(db->merch_db) == 1);
    ioopm_destroy_database(db);
}


void test_replenish()
{
    ioopm_db_t *db = ioopm_database_create();
    char *name1 = "a1";
    char *name2 = "a2";
    char *name3 = "a3";
    char *desc1 = "a1";
    char *desc2 = "a2";
    char *desc3 = "a3";
    char *shelf1 = "A00";
    int price = 0;
    CU_ASSERT_FALSE(ioopm_replenish_stock(db, name1, shelf1, 10));
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_add_merch(db, strdup(name2), strdup(desc2), price);
    ioopm_add_merch(db, strdup(name3), strdup(desc3), price);
    CU_ASSERT_TRUE(ioopm_replenish_stock(db, strdup(name1), strdup(shelf1), 10));
    elem_t merch1_key = {.str_value = name1};
    int quantity = get_quantity_for_merch(db, merch1_key);
    CU_ASSERT_TRUE(quantity == 10);
    CU_ASSERT_FALSE(ioopm_replenish_stock(db, (name1), (shelf1), 10));
    quantity = get_quantity_for_merch(db, merch1_key);
    CU_ASSERT_TRUE(quantity == 20);
    ioopm_destroy_database(db);
}

void test_create_and_remove_cart()
{
    ioopm_db_t *db = ioopm_database_create();
    ioopm_remove_cart(db, 0);
    CU_ASSERT_TRUE(db->number_of_carts == 0);
    ioopm_create_cart(db);
    ioopm_create_cart(db);
    ioopm_create_cart(db);
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(db->carts));
    CU_ASSERT_TRUE(db->number_of_carts == 3);
    ioopm_remove_cart(db, 0);
    ioopm_remove_cart(db, 4);
    CU_ASSERT_TRUE(db->number_of_carts == 3);
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(db->carts));
    ioopm_remove_cart(db, 1);
    ioopm_remove_cart(db, 2);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(db->carts));
    ioopm_destroy_database(db);
}

void test_add_to_cart()
{
    ioopm_db_t *db = ioopm_database_create();
    ioopm_create_cart(db);
    char *name1 = "a1";
    char *desc1 = "a1";
    char *shelf1 = "A00";
    int price = 0;
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_add_to_cart(db, ("stupid"), 10, 0);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(ioopm_linked_list_get(db->carts, 0).cart_value->order));
    ioopm_add_to_cart(db, (name1), 10, 0);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(ioopm_linked_list_get(db->carts, 0).cart_value->order));
    ioopm_replenish_stock(db, strdup(name1), strdup(shelf1), 10);
    ioopm_add_to_cart(db, strdup(name1), 10, 0);
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(ioopm_linked_list_get(db->carts, 0).cart_value->order));
    ioopm_destroy_database(db);
}

void test_remove_from_cart()
{
    ioopm_db_t *db = ioopm_database_create();
    ioopm_create_cart(db);
    char *name1 = "a1";
    char *desc1 = "a1";
    char *shelf1 = "A00";
    int price = 0;
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_replenish_stock(db, strdup(name1), strdup(shelf1), 10);
    ioopm_add_to_cart(db, strdup(name1), 10, 0);
    ioopm_remove_from_cart(db, 0, name1, 7);
    ioopm_list_t *order_list = (ioopm_linked_list_get(db->carts, 0).cart_value->order);
    ioopm_order_t *first_order = ioopm_linked_list_get(order_list, 0).order_value;
    CU_ASSERT_TRUE(strcmp(first_order->merch_name, name1)==0);
    CU_ASSERT_TRUE(first_order->quantity == 3);
    ioopm_remove_from_cart(db, 0, name1, 700);
    CU_ASSERT_TRUE(first_order->quantity == 0);
    ioopm_destroy_database(db);
}

void test_calculate_cost()
{
    ioopm_db_t *db = ioopm_database_create();
    int cost = ioopm_calculate_cost(db, 0);
    CU_ASSERT_TRUE(cost == 0);
    ioopm_create_cart(db);
    cost = ioopm_calculate_cost(db, 0);
    CU_ASSERT_TRUE(cost == 0);
    char *name1 = "a1";
    char *desc1 = "a1";
    char *shelf1 = "A00";
    char *name2 = "a2";
    char *desc2 = "a2";
    char *shelf2 = "A01";
    int price = 5;
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_add_merch(db, strdup(name2), strdup(desc2), price);
    ioopm_replenish_stock(db, strdup(name1), strdup(shelf1), 10);
    ioopm_replenish_stock(db, strdup(name2), strdup(shelf2), 10);
    ioopm_add_to_cart(db, strdup(name1), 10, 0);
    ioopm_add_to_cart(db, strdup(name2), 100, 0);
    cost = ioopm_calculate_cost(db, 0);
    CU_ASSERT_TRUE(cost == 100);
    ioopm_destroy_database(db);
}

void test_checkout()
{
    ioopm_db_t *db = ioopm_database_create();
    ioopm_create_cart(db);
    char *name1 = "a1";
    char *desc1 = "a1";
    char *shelf1 = "A00";
    char *name2 = "a2";
    char *desc2 = "a2";
    char *shelf2 = "A01";
    int price = 5;
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_add_merch(db, strdup(name2), strdup(desc2), price);
    ioopm_replenish_stock(db, strdup(name1), strdup(shelf1), 10);
    ioopm_replenish_stock(db, strdup(name2), strdup(shelf2), 10);
    ioopm_add_to_cart(db, strdup(name1), 5, 0);
    ioopm_add_to_cart(db, strdup(name2), 10, 0);
    ioopm_checkout(db, 0);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(db->carts));
    elem_t merch1 = {.str_value = name1};
    elem_t merch2 = {.str_value = name2};
    CU_ASSERT_TRUE(get_quantity_for_merch(db, merch1)==5);
    CU_ASSERT_TRUE(get_quantity_for_merch(db, merch2)==0);
    ioopm_destroy_database(db);
}

void test_checkout_possible()
{
    ioopm_db_t *db = ioopm_database_create();
    ioopm_create_cart(db);
    ioopm_create_cart(db);
    char *name1 = "a1";
    char *desc1 = "a1";
    char *shelf1 = "A00";
    char *name2 = "a2";
    char *desc2 = "a2";
    char *shelf2 = "A01";
    int price = 5;
    ioopm_add_merch(db, strdup(name1), strdup(desc1), price);
    ioopm_add_merch(db, strdup(name2), strdup(desc2), price);
    ioopm_replenish_stock(db, strdup(name1), strdup(shelf1), 10);
    ioopm_replenish_stock(db, strdup(name2), strdup(shelf2), 10);
    ioopm_add_to_cart(db, strdup(name1), 5, 0);
    ioopm_add_to_cart(db, strdup(name2), 10, 0);
    ioopm_add_to_cart(db, strdup(name1), 5, 1);
    ioopm_add_to_cart(db, strdup(name2), 10, 1);
    CU_ASSERT_TRUE(ioopm_checkout_possible(db, 0));
    CU_ASSERT_TRUE(ioopm_checkout_possible(db, 1));
    ioopm_checkout(db, 0);
    CU_ASSERT_FALSE(ioopm_checkout_possible(db, 1));
    ioopm_destroy_database(db);
}

int main()
{
    CU_pSuite test_suite_backend = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    test_suite_backend = CU_add_suite("test_suite_backend", init_suite, clean_suite);
    if (NULL == test_suite_backend)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (NULL == CU_add_test(test_suite_backend, "add_merch", test_add_merch)) /* ||
      (NULL == CU_add_test(test_suite_backend, "test_remove_merch", test_remove_merch))||
      (NULL == CU_add_test(test_suite_backend, "test_replenish", test_replenish))||
      (NULL == CU_add_test(test_suite_backend, "test_edit_merch", test_edit_merch))||
      (NULL == CU_add_test(test_suite_backend, "test_create_and_remove_cart", test_create_and_remove_cart))||
      (NULL == CU_add_test(test_suite_backend, "test_add_to_cart", test_add_to_cart))||
      (NULL == CU_add_test(test_suite_backend, "test_remove_from_cart", test_remove_from_cart))||
      (NULL == CU_add_test(test_suite_backend, "test_calculate_cost", test_calculate_cost))||
      (NULL == CU_add_test(test_suite_backend, "test_checkout", test_checkout))||
      (NULL == CU_add_test(test_suite_backend, "test_checkout_possible", test_checkout_possible))
									     */
    )

    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

