#include "CUnit/Basic.h"
#include <stdbool.h>
#include <stdlib.h>
#include "user_interface.h"
#include "business_logic.h"
#include "hash_table.h"
#include "utils.h"
#include "linked_list.h"
#include "common.h"
#include "refmem.h"


void test_add_merch()
{
  //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  
  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");

  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space, Varm, 200, NULL);
  
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);
  


  //Tests//
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "Boll"}).success);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "Solros"}).success); 
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = " "}).success);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "Skog"}).success);

 
  char *desc = merch_getdesc(warehouse, "Boll");
  CU_ASSERT_STRING_EQUAL("Rund", desc);

  desc = merch_getdesc(warehouse, "Skog");
  CU_ASSERT_STRING_EQUAL("Träd", desc);

  desc = merch_getdesc(warehouse, "Solros");
  CU_ASSERT_STRING_EQUAL("Gul", desc);

  
  warehouse_destroy(warehouse, NULL, NULL);
}


void test_replenish_merch()
{
   //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
 //ADD merchendise to warehouse
  
 //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space, Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);;

  char *A25 = strdup("A25");
  char *B12 = strdup("B12");
  char *Z51 = strdup("Z51");
  //Replenishar
  warehouse_store_location(warehouse, shelf_db, A25, (elem_t) { .str_value = "Boll"}, 5);
  warehouse_store_location(warehouse, shelf_db, B12, (elem_t) { .str_value = "Skog"}, 1);
  warehouse_store_location(warehouse, shelf_db, Z51, (elem_t) { .str_value = "Solros"}, 1);
  
  //Kollar att shelf_db innehåller namn till respektive hylla
  char *merch_name = ioopm_hash_table_lookup(shelf_db, (elem_t) { .str_value = A25}).element.str_value;
  CU_ASSERT_STRING_EQUAL("Boll", merch_name);
  
  merch_name = ioopm_hash_table_lookup(shelf_db, (elem_t) { .str_value = B12}).element.str_value;
  CU_ASSERT_STRING_EQUAL("Skog", merch_name);

  merch_name = ioopm_hash_table_lookup(shelf_db, (elem_t) { .str_value = Z51}).element.str_value;
  CU_ASSERT_STRING_EQUAL("Solros", merch_name);

  //Need to re-strdup
  char *newshelf = strdup("A25");
  char *newshelf2 = strdup("B12");
  
  //Replenishar  stock på A25 där Boll redan finns med 2, B12 ökas med 4.
  warehouse_store_location(warehouse, shelf_db, newshelf, (elem_t) { .str_value = "Boll"}, 2);
  warehouse_store_location(warehouse, shelf_db, newshelf2, (elem_t) { .str_value = "Skog"}, 4);
  
  //Kollar att amount i warehouse på A25, B12 stämmer
  int amountA25 = getAmountSingel(warehouse, "Boll", 0);
  CU_ASSERT_EQUAL(7, amountA25);
  
  int amountB12 = getAmountSingel(warehouse, "Skog", 0);
  CU_ASSERT_EQUAL(5, amountB12);
  
  warehouse_destroy(warehouse, shelf_db, NULL);
}

void test_edit_merch()
{
  //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
 //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space, Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *skagg =strdup("skägg");
  char *tomte =strdup("tomte");
  char *julgran = strdup("julgran");
  char *plast = strdup("plast");
  //Byter ut Skog till tomte
  warehouse_edit_merchandise_with_fields(warehouse, shelf_db, tomte, skagg, 200, (elem_t) { .str_value = "Skog"}, list_of_carts);

  warehouse_edit_merchandise_with_fields(warehouse, shelf_db, julgran, plast, 200, (elem_t) { .str_value = "Leksak"}, list_of_carts);

  //Kollar att tomte finns och att Skog inte finns
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "tomte"}).success);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "Skog"}).success);
  
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "julgran"}).success);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "Leksak"}).success);
   warehouse_destroy(warehouse, shelf_db, list_of_carts);
}


void test_remove_merch()
{
  //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);
  
  char *A25 = strdup("A25");
  char *B12 = strdup("B12");
  //Replenish
  warehouse_store_location(warehouse, shelf_db, A25, (elem_t) { .str_value = "Boll"}, 5);
  warehouse_store_location(warehouse, shelf_db, B12, (elem_t) { .str_value = "Skog"}, 1);
  
  
  //Check that the hashtable shelf_db contains all the names of the shelves added 
  char *merch_name = ioopm_hash_table_lookup(shelf_db, (elem_t) { .str_value = "A25"}).element.str_value;
  CU_ASSERT_STRING_EQUAL("Boll", merch_name);
  merch_name = ioopm_hash_table_lookup(shelf_db, (elem_t) { .str_value = "B12"}).element.str_value;
  CU_ASSERT_STRING_EQUAL("Skog", merch_name);
  
  //Removes items and checks that they dont exist in warehouse anymore
  warehouse_remove_merch_with_key(warehouse, shelf_db, (elem_t) { .str_value = "Boll"}, list_of_carts);
  warehouse_remove_merch_with_key(warehouse, shelf_db, (elem_t) { .str_value = "Skog"}, list_of_carts);
  
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "Boll"}).success);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(warehouse,(elem_t) { .str_value = "Skog"}).success);
  
  warehouse_remove_merch_with_key(warehouse, shelf_db, (elem_t) { .str_value = "Solros"}, list_of_carts);
  warehouse_remove_merch_with_key(warehouse, shelf_db, (elem_t) { .str_value = " "}, list_of_carts);
  
  CU_ASSERT_EQUAL(1, ioopm_hash_table_size(warehouse));
  
  warehouse_remove_merch_with_key(warehouse, shelf_db, (elem_t) { .str_value = "Leksak"}, list_of_carts);
  
  CU_ASSERT_EQUAL(0, ioopm_hash_table_size(warehouse));
  CU_ASSERT_EQUAL(0, ioopm_hash_table_size(shelf_db));

  warehouse_destroy(warehouse, shelf_db, list_of_carts);
}

void test_add_to_cart()
{
  //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *shelf1 = strdup("A25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");

  //Replenish
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = Boll}, 5);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = Skog}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = Solros}, 10);

  //Crates a cart and adds 4 "Boll" to it and 1 "Skog" to IT
  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Boll}, 4);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Skog}, 1);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Solros},3);

  //Check that amount of "boll" = 5. Amount of skog = 1. Amount of solros = 3
  int amount = getAmountSingel(warehouse, Boll, 0);
  CU_ASSERT_EQUAL(5, amount);

  amount = getAmountSingel(warehouse, Skog, 0);
  CU_ASSERT_EQUAL(3, amount);

  amount = getAmountSingel(warehouse, Solros, 0);
  CU_ASSERT_EQUAL(10, amount);

  warehouse_destroy(warehouse, shelf_db, list_of_carts);
}

void test_calculate_cost()
{
    //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 10, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 5, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  //Replenish

  char *shelf1 = strdup("A25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");
  
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = Boll}, 5);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = Skog}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = Leksak}, 10);

  
  //Creates a cart and adds 4 "BOll" to it, 1 "Skog" to it, 
  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Boll}, 4);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Skog}, 1);

  size_t totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 90);
  
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Skog}, 1);
  totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 140);
  
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Leksak}, 2);
  totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 150);

  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Leksak}, 2);
  totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 160);

  //We make leksak have 0 in stock
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = Leksak}, 6);
  totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 190);

  warehouse_destroy(warehouse, shelf_db, list_of_carts);
  
}

void test_checkout()
{
      //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 10, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 5, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *shelf1 = strdup("C25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");
  
  //Replenish stock
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = "Boll"}, 5);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = "Skog"}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = "Leksak"}, 10);


  //Creates a cart and adds 5 "BOll" to it, 3 "Skog" to it, 
  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Boll"}, 4);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Skog"}, 1);
  
  //Check amount of merches are correct before checkout
  int amount = getAmountSingel(warehouse, "Boll", 0);
  CU_ASSERT_EQUAL(5, amount);
  
  amount = getAmountSingel(warehouse, "Skog", 0);
  CU_ASSERT_EQUAL(3, amount);

  amount = getAmountSingel(warehouse, "Leksak", 0);
  CU_ASSERT_EQUAL(10, amount);
  
  //Checkout
  warehouse_checkout_cart_aux(warehouse,list_of_carts, 0, shelf_db);
  
  //Check that amount merches are correct after checkout
   amount = getAmountSingel(warehouse, "Boll", 0);
  CU_ASSERT_EQUAL(1, amount);
  
  amount = getAmountSingel(warehouse, "Skog", 0);
  CU_ASSERT_EQUAL(2, amount);

  amount = getAmountSingel(warehouse, "Leksak", 0);
  CU_ASSERT_EQUAL(10, amount);
  
  //Creates a second cart adds 1 skog to it, and 3 Leksak
  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Skog"}, 2);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Leksak"}, 3);

  //Checkouts second cart
  warehouse_checkout_cart_aux(warehouse, list_of_carts, 0, shelf_db);
  //Check that amount merches are correct after checkout
  
  amount = getAmountSingel(warehouse, Boll, 0);
  CU_ASSERT_EQUAL(1, amount);
  
  amount = getAmountSingel(warehouse, Skog, 0);
  CU_ASSERT_EQUAL(0, amount);

  amount = getAmountSingel(warehouse, Leksak, 0);
  CU_ASSERT_EQUAL(7, amount);
  
  warehouse_destroy(warehouse, shelf_db, list_of_carts);
}

void test_key_has_locations()
{
     //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");

  
 //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space, Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *A25 = strdup("A25");
  char *B12 = strdup("B12");
  //Replenishar
  warehouse_store_location(warehouse, shelf_db, A25, (elem_t) { .str_value = "Boll"}, 5);
  warehouse_store_location(warehouse, shelf_db, B12, (elem_t) { .str_value = "Skog"}, 1);

  CU_ASSERT_TRUE(warehouse_key_has_locations(warehouse, (elem_t) { .str_value = "Skog"}));
  CU_ASSERT_TRUE(warehouse_key_has_locations(warehouse, (elem_t) { .str_value = "Boll"}));
  CU_ASSERT_FALSE(warehouse_key_has_locations(warehouse, (elem_t) { .str_value = "Leksak"}));
  CU_ASSERT_FALSE(warehouse_key_has_locations(warehouse, (elem_t) { .str_value = " "}));
  
  
  warehouse_destroy(warehouse, shelf_db, NULL);
}

void test_shelf_available()
{
      //Create warehouse
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();



  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");

  
 //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space, Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *A25 = strdup("A25");
  char *B12 = strdup("B12");
  //Replenishar
  warehouse_store_location(warehouse, shelf_db, A25, (elem_t) { .str_value = "Boll"}, 5);
  warehouse_store_location(warehouse, shelf_db, B12, (elem_t) { .str_value = "Skog"}, 1);

  CU_ASSERT_TRUE(is_shelf_available(warehouse, shelf_db, "Y05", (elem_t) { .str_value = "Boll"}));
  CU_ASSERT_TRUE(is_shelf_available(warehouse, shelf_db, "A25", (elem_t) { .str_value = "Boll"}));
  CU_ASSERT_TRUE(is_shelf_available(warehouse, shelf_db, "B12", (elem_t) { .str_value = "Skog"}));
  
  
  warehouse_destroy(warehouse, shelf_db, NULL);
}

void test_amount_add_available()
{
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");

  
 //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space, Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *A25 = strdup("A25");
  char *B12 = strdup("B12");
  //Replenishar
  warehouse_store_location(warehouse, shelf_db, A25, (elem_t) { .str_value = "Boll"}, 5);
  warehouse_store_location(warehouse, shelf_db, B12, (elem_t) { .str_value = "Skog"}, 1);

  CU_ASSERT_TRUE(amount_to_add_available(warehouse, (elem_t) { .str_value = "Boll"}, 1));
  CU_ASSERT_TRUE(amount_to_add_available(warehouse, (elem_t) { .str_value = "Boll"}, 4));
  CU_ASSERT_FALSE(amount_to_add_available(warehouse, (elem_t) { .str_value = "Skog"}, 3));
  CU_ASSERT_TRUE(amount_to_add_available(warehouse, (elem_t) { .str_value = "Skog"}, 1));

  warehouse_destroy(warehouse, shelf_db, NULL);
}

void test_is_merch_stocked()
{
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");

  
 //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 1, NULL);
  warehouse_add_merch_with_fields(warehouse, space, Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 54, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *A25 = strdup("A25");
  char *B12 = strdup("B12");
  //Replenishar
  warehouse_store_location(warehouse, shelf_db, A25, (elem_t) { .str_value = "Boll"}, 5);
  warehouse_store_location(warehouse, shelf_db, B12, (elem_t) { .str_value = "Skog"}, 1);

  CU_ASSERT_TRUE(is_merch_stocked(warehouse, (elem_t) { .str_value = "Skog"}));
  CU_ASSERT_TRUE(is_merch_stocked(warehouse, (elem_t) { .str_value = "Boll"}));
  CU_ASSERT_FALSE(is_merch_stocked(warehouse, (elem_t) { .str_value = "Leksak"}));
  CU_ASSERT_FALSE(is_merch_stocked(warehouse, (elem_t) { .str_value = " "}));

  warehouse_destroy(warehouse, shelf_db, NULL);
}

void test_size_of_cart()
{

  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 10, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 5, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *shelf1 = strdup("C25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");
  
  //Replenish stock
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = "Boll"}, 7);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = "Skog"}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = "Leksak"}, 10);

  warehouse_list_locations(warehouse,(elem_t) { .str_value = "Boll"});

  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Boll"}, 4);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Skog"}, 1);

  CU_ASSERT_EQUAL(2, warehouse_size_of_cart(list_of_carts, 0));

  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Boll"}, 1);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Leksak"}, 1);
  CU_ASSERT_EQUAL(3, warehouse_size_of_cart(list_of_carts, 0));
  //Kollar att en tom cart har size 0
  CU_ASSERT_EQUAL(0, warehouse_size_of_cart(list_of_carts, 1));
  warehouse_add_cart(list_of_carts);
  warehouse_add_cart(list_of_carts);
  CU_ASSERT_EQUAL(0, warehouse_size_of_cart(list_of_carts, 2));

  warehouse_destroy(warehouse, shelf_db, list_of_carts);
  
}

void test_is_cart_empty()
{
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 10, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 5, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *shelf1 = strdup("C25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");
  
  //Replenish stock
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = "Boll"}, 7);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = "Skog"}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = "Leksak"}, 10);

  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Boll"}, 6);
  warehouse_add_cart(list_of_carts);

  CU_ASSERT_TRUE(is_cart_empty(list_of_carts, 1))
  warehouse_add_cart(list_of_carts);
  CU_ASSERT_TRUE(is_cart_empty(list_of_carts, 2));
  CU_ASSERT_FALSE(is_cart_empty( list_of_carts, 0));

  warehouse_destroy(warehouse, shelf_db, list_of_carts);
}

void test_remove_cart_merch_with_index()
{
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 10, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 5, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *shelf1 = strdup("C25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");
  
  //Replenish stock
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = "Boll"}, 7);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = "Skog"}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = "Leksak"}, 10);

  warehouse_add_cart(list_of_carts);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Boll"}, 4);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Skog"}, 1);

  warehouse_list_cart_merch(list_of_carts, 0);
  CU_ASSERT_EQUAL(2, warehouse_size_of_cart(list_of_carts, 0));
  int totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 90);

  remove_cart_merch_with_index(list_of_carts, 0, 0, 2);
  totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 70);


  remove_cart_merch_with_index(list_of_carts, 0, 0, 5);

  totalCost = warehouse_total_cost_of_cart_with_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(totalCost, 50);

  warehouse_destroy(warehouse, shelf_db, list_of_carts);
  }


void test_get_cart_index()
{
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");
  
  //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 10, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 5, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *shelf1 = strdup("C25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");
  
  //Replenish stock
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = "Boll"}, 7);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = "Skog"}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = "Leksak"}, 10);

  warehouse_add_cart(list_of_carts);
  int cartIndex = get_cart_index(list_of_carts, 0);
  CU_ASSERT_EQUAL(cartIndex, 1);
  cartIndex = get_cart_index(list_of_carts, 10);
  CU_ASSERT_EQUAL(cartIndex, 0);
  warehouse_destroy(warehouse, shelf_db, list_of_carts);
}

void test_remove_with_cart()
{
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();

  char *Boll = strdup("Boll");    char *Rund = strdup ("Rund");
  char *space = strdup(" ");      char *Varm = strdup ("Varm");
  char *Skog = strdup("Skog");    char *Trad = strdup ("Träd");
  char *Leksak = strdup("Leksak");char *Rod = strdup ("Röd");
  char *Solros = strdup("Solros");char *Gul = strdup ("Gul");

    //ADD merchendise to warehouse
  warehouse_add_merch_with_fields(warehouse, Boll, Rund, 10, NULL);
  warehouse_add_merch_with_fields(warehouse, space , Varm, 200, NULL);
  warehouse_add_merch_with_fields(warehouse, Skog, Trad, 50, NULL);
  warehouse_add_merch_with_fields(warehouse, Leksak, Rod, 5, NULL);
  warehouse_add_merch_with_fields(warehouse, Solros, Gul, 5, NULL);

  char *shelf1 = strdup("C25");
  char *shelf2 = strdup("B12");
  char *shelf3 = strdup("Z51");
  
  //Replenish stock
  warehouse_store_location(warehouse, shelf_db, shelf1, (elem_t) { .str_value = "Boll"}, 7);
  warehouse_store_location(warehouse, shelf_db, shelf2, (elem_t) { .str_value = "Skog"}, 3);
  warehouse_store_location(warehouse, shelf_db, shelf3, (elem_t) { .str_value = "Leksak"}, 10);

  warehouse_add_cart(list_of_carts);
  warehouse_add_cart(list_of_carts);
  warehouse_add_cart(list_of_carts);
  
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Boll"}, 4);
  
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Skog"}, 1);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 1, (elem_t) { .str_value = "Leksak"}, 3);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 2, (elem_t) { .str_value = "Leksak"}, 4);

  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 0),  2);
  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 1),  1);
  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 2),  1);

  warehouse_remove_merch_with_key(warehouse, shelf_db, (elem_t) { .str_value = "Boll"}, list_of_carts);
  
  //int cart_size = warehouse_size_of_cart(list_of_carts, 0);

  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 0),  1);
  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 1),  1);
  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 2),  1);

  warehouse_remove_merch_with_key(warehouse, shelf_db, (elem_t) { .str_value = "Leksak"}, list_of_carts);

  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 0),  1);
  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 1),  0);
  CU_ASSERT_EQUAL(warehouse_size_of_cart(list_of_carts, 2),  0);

  char *Boll2 = strdup("Boll");
  char *Rund2 = strdup ("Rund");
  //re-add item that was removed
  warehouse_add_merch_with_fields(warehouse, Boll2, Rund2, 10, NULL);
  //re-add shelf that was previously used
  char *shelf12 = strdup("C25");
  warehouse_store_location(warehouse, shelf_db, shelf12, (elem_t) { .str_value = "Boll"}, 7);
  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 0, (elem_t) { .str_value = "Boll"}, 8);

  add_merchandise_to_cart_with_index(warehouse, list_of_carts, 1, (elem_t) { .str_value = "Boll"}, 4);

  //Checkout cart 1 
  warehouse_checkout_cart_aux(warehouse, list_of_carts, 0, shelf_db);
  //Cant checkout this because not enough amount available
  warehouse_checkout_cart_aux(warehouse, list_of_carts, 0, shelf_db);
  //Cant checkout this because of empty cart
  warehouse_checkout_cart_aux(warehouse, list_of_carts, 4, shelf_db);
  
  warehouse_destroy(warehouse, shelf_db, list_of_carts);
}

int main()
{
  //Init testing
  CU_initialize_registry();
  
  CU_pSuite suite_base = CU_add_suite("Base functions", NULL, NULL);
  CU_add_test(suite_base, "add_merch", test_add_merch);
  /*
  CU_add_test(suite_base, "remove merch", test_remove_merch);
  CU_add_test(suite_base, "replenish merch", test_replenish_merch);
  CU_add_test(suite_base, "edit merch", test_edit_merch);
  CU_add_test(suite_base, "add to cart", test_add_to_cart);
  CU_add_test(suite_base, "calculate cost", test_calculate_cost);
  CU_add_test(suite_base, "checkout", test_checkout);

  CU_pSuite suite_static = CU_add_suite("Static functions", NULL, NULL);
  CU_add_test(suite_static, "warehouse_key_has_locations", test_key_has_locations);
  CU_add_test(suite_static, "is_shelf_available", test_shelf_available);
  CU_add_test(suite_static, "amount_to_add_available", test_amount_add_available);
  CU_add_test(suite_static, "is_merch_stocked", test_is_merch_stocked);
  CU_add_test(suite_static, "size_of_cart", test_size_of_cart);
  CU_add_test(suite_static, "is cart empty", test_is_cart_empty);
  // CU_add_test(suite_static, "remove cart_merch_with index", test_remove_cart_merch_with_index);

  //CU_add_test(suite_static, "get_cart_index", test_get_cart_index);
  CU_add_test(suite_static, "remove w/ cart", test_remove_with_cart);*/
    
  
  CU_basic_run_tests();
  CU_cleanup_registry();
  return 0;
}
