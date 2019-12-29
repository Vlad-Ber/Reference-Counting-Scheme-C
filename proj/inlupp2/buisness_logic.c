#ifndef _BLC_
#define _BLC_

#include <stdio.h>
#include <stdlib.h>
#include "list_linked.h"
#include "iterator.h"
#include "hash_table.h"
#include "utils.h"
#include "common.h"
#include "../refmem.h"
#include "buisness_logic.c"


#define Free(ptr) {free(ptr); ptr = NULL;}
 
struct merch
{
  char *name;
  char *description;
  int price;
  ioopm_list_t *shelf;
};

struct shelf
{
  char *shelf_name;
  int quantity;
};

struct order
{
  char *merch_name;
  int quantity;
};

struct cart
{
  int id;
  ioopm_list_t *order;
};

struct database
{
  ioopm_hash_table_t *merch_db;
  ioopm_hash_table_t *shelf_db;
  ioopm_list_t *carts;
  int number_of_carts;
};


static void apply_destroy_order(int ignored, elem_t *order, void *extra)
{
  free(order->order_value->merch_name);
  free(order->order_value);
}

static void destroy_orders(ioopm_list_t *orders)
{
  ioopm_linked_apply_to_all(orders, apply_destroy_order, NULL);
  ioopm_linked_list_destroy(orders);
}

static void apply_destroy_cart(int ignored, elem_t *cart, void *extra)
{
  destroy_orders(cart->cart_value->order);
  free(cart->cart_value);
}

static void apply_destroy_shelfs(int ignored, elem_t *shelf, void *extra)
{
  free(shelf->shelf_value->shelf_name);
  free(shelf->shelf_value);
}

static void apply_destroy_shelf_name(elem_t shelf_name, elem_t *value, void *extra)
{
  free(shelf_name.str_value);
  free(value->str_value);
}

static void shelf_list_destroy(ioopm_list_t* shelfs)
{
  ioopm_linked_apply_to_all(shelfs, apply_destroy_shelfs, NULL); 
}
static void destroy_merch(ioopm_merch_t *merch_to_destroy)
{
  puts("print");
  free(merch_to_destroy->name);
  free(merch_to_destroy->description);
  shelf_list_destroy(merch_to_destroy->shelf);
  ioopm_linked_list_destroy(merch_to_destroy->shelf);
  free(merch_to_destroy);
  puts("destroy_merch done");
}

static void apply_destroy_merch(elem_t merch, elem_t *merch_info, void *extra)
{
  destroy_merch(merch_info->merch_value);
}







static void carts_destroy(ioopm_db_t *db)
{
  ioopm_linked_apply_to_all(db->carts, apply_destroy_cart, NULL);
  ioopm_linked_list_destroy(db->carts);
}


static void merch_db_destroy(ioopm_db_t *database)
{
  ioopm_hash_table_apply_to_all(database->merch_db, apply_destroy_merch, NULL);
  ioopm_hash_table_destroy(database->merch_db);
}


static void shelf_db_destroy(ioopm_db_t *database)
{
  ioopm_hash_table_apply_to_all(database->shelf_db, apply_destroy_shelf_name, NULL);
  ioopm_hash_table_destroy(database->shelf_db);
}

void ioopm_destroy_database(void *db)
{
  merch_db_destroy((ioopm_db_t*)db);
  shelf_db_destroy((ioopm_db_t*)db);
  carts_destroy((ioopm_db_t*)db);
  free((ioopm_db_t*)db);
}

static int string_knr_hash(elem_t str)
{
  int result = 0;
  do
    {
      result = result * 31 + *str.str_value;
    }
  while (*++str.str_value != '\0');
  if(result < 0) result = -result;
  return result;
}
static elem_t get_value_for_merch(ioopm_db_t *database, elem_t merch)
{
  elem_t merch_information;
  elem_t *merch_information_ptr = &merch_information;
  ioopm_hash_table_lookup(database->merch_db, merch, merch_information_ptr);
  return merch_information;
}

static void remove_specific_merch(ioopm_db_t *database, elem_t merch)
{
  elem_t value = get_value_for_merch(database, merch);
  ioopm_hash_table_remove(database->merch_db, merch);
  destroy_merch(value.merch_value);
}



void ioopm_remove_merch(ioopm_db_t *database, char *merch_to_remove)
{
  elem_t key = {.str_value = merch_to_remove};
  elem_t merch;
  elem_t *merch_ptr = &merch;
  if(ioopm_hash_table_lookup(database->merch_db, key, merch_ptr))
    {
      remove_specific_merch(database, key);
    }
}


ioopm_db_t *ioopm_database_create()
{
  ioopm_db_t *db = allocate(sizeof(ioopm_db_t), (void (*)(void *)) ioopm_destroy_database);
  db->merch_db = ioopm_hash_table_create(ioopm_eq_function_str, ioopm_eq_function_int, string_knr_hash);
  db->shelf_db = ioopm_hash_table_create(ioopm_eq_function_str, ioopm_eq_function_int, string_knr_hash);
  db->carts = ioopm_linked_list_create(ioopm_eq_function_str);
  db->number_of_carts = 0;
  return db;
}


static int get_quantity_for_merch(ioopm_db_t *database, elem_t merch)
{
  int result = 0;
  elem_t merch_info = get_value_for_merch(database, merch);
  ioopm_list_t *shelfs = merch_info.merch_value->shelf;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(shelfs);
  while(ioopm_iterator_has_next(iter))
    {
      ioopm_iterator_next(iter);
      elem_t current = ioopm_iterator_current(iter);
      result += current.shelf_value->quantity;
    }
  ioopm_iterator_destroy(iter);
  return result;
}

static bool merch_exists(ioopm_db_t *db, elem_t merch_to_lookup)
{
  elem_t merch_information;
  elem_t *merch_information_ptr = &merch_information;
  return ioopm_hash_table_lookup(db->merch_db, merch_to_lookup, merch_information_ptr);
}





ioopm_merch_t *ioopm_create_merch(char *name, char *desc, int price)
{
  ioopm_merch_t *new_merch = allocate(sizeof(ioopm_merch_t), (void (*)(void *)) ioopm_remove_merch);
  new_merch->shelf = ioopm_linked_list_create(ioopm_eq_function_str);
  new_merch->name = name;
  new_merch->description = desc;
  new_merch->price = price;
  return new_merch;
}




void ioopm_add_merch(ioopm_db_t *database, char *name, char *description, int price)
{
  elem_t merch_key = {.str_value = name};
  elem_t ignored_value;
  if(!ioopm_hash_table_lookup(database->merch_db, merch_key, &ignored_value))
    {
      ioopm_merch_t *new_merch = ioopm_create_merch(name, description, price);
      elem_t merch_information = {.merch_value = new_merch};
      ioopm_hash_table_insert(database->merch_db, merch_key, merch_information);
    }
}





static void insert_merch(ioopm_db_t *database, ioopm_merch_t *new_merch)
{
  elem_t merch_information = {.merch_value = new_merch};
  elem_t merch_key = {.str_value = new_merch->name};
  ioopm_hash_table_insert(database->merch_db, merch_key, merch_information);
}

bool ioopm_edit_merch(ioopm_db_t *database, char *merch_name, char *new_name, char *new_desc, int new_price)
{
  elem_t key = {.str_value = merch_name};
  elem_t new_key = {.str_value = new_name};
  elem_t saved_merch;
  elem_t *saved_merch_ptr = &saved_merch;
  if (ioopm_hash_table_lookup(database->merch_db, key, saved_merch_ptr)  && !merch_exists(database, new_key))
    {
      ioopm_list_t *merch_shelfs = saved_merch_ptr->merch_value->shelf;
      ioopm_list_t *list_to_destroy = ioopm_linked_list_create(ioopm_eq_function_str);
      saved_merch_ptr->merch_value->shelf = list_to_destroy;
      remove_specific_merch(database, key);
      ioopm_merch_t *new_merch = ioopm_create_merch(new_name, new_desc, new_price);
      ioopm_linked_list_destroy(new_merch->shelf);
      new_merch->shelf = merch_shelfs;
      insert_merch(database, new_merch);
      return true;
    }
  return false;
}

static ioopm_shelf_t *create_shelf(char *shelf_name, int quantity)
{
  ioopm_shelf_t *shelf = calloc(1, sizeof(ioopm_shelf_t));
  shelf->shelf_name = shelf_name;
  shelf->quantity = quantity;
  return shelf;
}



static void add_shelf_to_db(ioopm_db_t *database, elem_t merch, int quantity, elem_t shelf_name)
{ 
  ioopm_shelf_t *new_shelf = create_shelf(shelf_name.str_value, quantity);
  
  elem_t value_merch = get_value_for_merch(database, merch);
  
  ioopm_list_t *list_shelfs = value_merch.merch_value->shelf;
  
  elem_t shelf_to_add_to_list = {.shelf_value = new_shelf};
  elem_t shelf_to_add_key = {.str_value = strdup(new_shelf->shelf_name)};
  
  ioopm_linked_list_append(list_shelfs, shelf_to_add_to_list);
  
  ioopm_hash_table_insert(database->shelf_db, shelf_to_add_key, merch);
}

static ioopm_shelf_t *find_shelf(ioopm_list_t *list_shelfs, elem_t shelf_name)
{
  //ioopm_shelf_t *shelf = create_shelf("A00", 10);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list_shelfs);
  while (ioopm_iterator_has_next(iter))
    {
      ioopm_iterator_next(iter);
      elem_t current_elem = ioopm_iterator_current(iter);
      ioopm_shelf_t *current_shelf = current_elem.shelf_value;
      char *current_shelf_name = current_shelf->shelf_name;
      if (strcmp(current_shelf_name, shelf_name.str_value)==0)
	{
	  ioopm_iterator_destroy(iter);
	  return current_shelf;
	}
    }
  ioopm_iterator_destroy(iter);
  return NULL;
}

static void increase_merch_quantity(ioopm_db_t *database, elem_t merch, int quantity, elem_t shelf_name)
{
  elem_t merch_value = get_value_for_merch(database, merch);
  ioopm_list_t *list_shelfs = merch_value.merch_value->shelf;
  ioopm_shelf_t *shelf_to_replenish = find_shelf(list_shelfs, shelf_name);
  shelf_to_replenish->quantity += quantity;
}

bool ioopm_replenish_stock(ioopm_db_t *database, char *merch_name, char *shelf_name, int no_items)
{
  elem_t merch_to_replenish = {.str_value = merch_name};
  elem_t shelf_to_replenish = {.str_value = shelf_name};
  elem_t saved_merch;
  elem_t *saved_merch_ptr = &saved_merch;

  bool merch_exists = ioopm_hash_table_lookup(database->merch_db, merch_to_replenish, saved_merch_ptr);
  bool shelf_exists = ioopm_hash_table_lookup(database->shelf_db, shelf_to_replenish, saved_merch_ptr);
  
  if (!shelf_exists && merch_exists && no_items > 0)
    {
      add_shelf_to_db(database, merch_to_replenish, no_items, shelf_to_replenish);
      return true;
    }
  else if (shelf_exists && merch_exists && no_items > 0) 
    {
      bool shelf_has_same_merch = (strcmp(saved_merch_ptr->str_value, merch_to_replenish.str_value) == 0);
      if (shelf_has_same_merch)
	{
	  increase_merch_quantity(database, merch_to_replenish, no_items, shelf_to_replenish);
	}
      return false;
    }
  else
    {
      return false;
    }
}

void ioopm_create_cart(ioopm_db_t *db)
{
  ioopm_cart_t *new_cart = calloc(1, sizeof(ioopm_cart_t));
  ioopm_list_t *order = ioopm_linked_list_create(ioopm_eq_function_str);
  new_cart->id = db->number_of_carts;
  new_cart->order = order;
  elem_t cart_to_add = {.cart_value = new_cart};
  ioopm_linked_list_append(db->carts, cart_to_add);
  db->number_of_carts += 1;
}

static ioopm_order_t *create_order(char *merch_name, int quantity)
{
  ioopm_order_t *new_order = calloc(1, sizeof(ioopm_order_t));
  new_order->merch_name = merch_name;
  new_order->quantity = quantity;
  return new_order;
}


static bool remove_cart_aux(ioopm_db_t *db, int cart_id, ioopm_cart_t *cart)
{

  ioopm_list_t *carts = db->carts;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(carts);
  int index = 0;
  while(ioopm_iterator_has_next(iter))
    {
      ioopm_iterator_next(iter);
      elem_t current_cart = ioopm_iterator_current(iter);
      if (current_cart.cart_value->id == cart_id)
	{
	  destroy_orders(current_cart.cart_value->order);
	  ioopm_linked_list_remove(carts, index);
	  free(current_cart.cart_value);
	  ioopm_iterator_destroy(iter);
	  return true;
	}
      index++;
    }
  ioopm_iterator_destroy(iter);
  return false;
}

void ioopm_remove_cart(ioopm_db_t *db, int cart_id)
{
  ioopm_cart_t cart;
  ioopm_cart_t *cart_to_remove = &cart;
  remove_cart_aux(db, cart_id, cart_to_remove);
}


static bool find_cart(ioopm_db_t *db, int cart_id, elem_t *cart)
{
  ioopm_list_t *carts = db->carts;
  ioopm_list_iterator_t *iter = ioopm_list_iterator(carts);
  while(ioopm_iterator_has_next(iter))
    {
      ioopm_iterator_next(iter);
      elem_t current_cart = ioopm_iterator_current(iter);
      if (current_cart.cart_value->id == cart_id)
	{
	  cart->cart_value = current_cart.cart_value;
	  ioopm_iterator_destroy(iter);
	  return true;
	}
    }
  ioopm_iterator_destroy(iter);
  return false;
}


// Sätter quantity till 0 om man försöker ta bort för många
void ioopm_remove_from_cart(ioopm_db_t *db, int cart_id, char *merch_name, int quantity)
{
  elem_t cart;
  elem_t *cart_address = &cart;
  if (find_cart(db, cart_id, cart_address))
    {
      ioopm_list_iterator_t *iter = ioopm_list_iterator(cart_address->cart_value->order);
      while(ioopm_iterator_has_next(iter))
	{
	  ioopm_iterator_next(iter);
	  ioopm_order_t *current_order = ioopm_iterator_current(iter).order_value;
	  if (strcmp(current_order->merch_name, merch_name) == 0)
	    {
	      if (quantity <= current_order->quantity)
		{
		current_order->quantity = current_order->quantity - quantity;
		}
	      else
		{
		  current_order->quantity = 0;
		}
	    }
	}
      ioopm_iterator_destroy(iter);
    }

}

//Ta bort så många vi kan om quantity är för stor?
//get_available_quantity_for_merch?
bool ioopm_add_to_cart(ioopm_db_t *db, char *merch_name, int quantity, int cart_id)
{
  elem_t merch_to_add = {.str_value = merch_name};
  if(merch_exists(db, merch_to_add) && get_quantity_for_merch(db, merch_to_add) > 0 && cart_id >= 0)
    {
      if(quantity > get_quantity_for_merch(db, merch_to_add))
	{
	  quantity = get_quantity_for_merch(db, merch_to_add);
	}
      
      elem_t order_to_add = {.order_value = create_order(merch_to_add.str_value, quantity)};
      elem_t cart;
      elem_t *cart_address = &cart;
      if(find_cart(db, cart_id, cart_address))
	{
	  ioopm_linked_list_append(cart_address->cart_value->order, order_to_add);
	  return true;
	}
    }
  return false;
}

int ioopm_calculate_cost(ioopm_db_t *db, int cart_id)
{
  elem_t cart;
  elem_t *cart_address = &cart;
  if (find_cart(db, cart_id, cart_address))
    {
      int calculator = 0;
      ioopm_list_iterator_t *iter = ioopm_list_iterator(cart_address->cart_value->order);
      while (ioopm_iterator_has_next(iter))
	{
	  ioopm_iterator_next(iter);
	  ioopm_order_t *current_order = ioopm_iterator_current(iter).order_value;
	  elem_t merch_name = {.str_value = current_order->merch_name};
	  int price = get_value_for_merch(db, merch_name).merch_value->price;
	  calculator += (current_order->quantity * price);
	}
      ioopm_iterator_destroy(iter);
      return calculator;
    }
  return 0;
}

bool ioopm_checkout_possible(ioopm_db_t *db, int cart_id)
{
  elem_t cart;
  elem_t *cart_address = &cart;
  if(find_cart(db, cart_id, cart_address))
    {
      ioopm_list_iterator_t *iter = ioopm_list_iterator(cart_address->cart_value->order);
      while(ioopm_iterator_has_next(iter))
	{
	  ioopm_iterator_next(iter);
	  ioopm_order_t *current_order = ioopm_iterator_current(iter).order_value;
	  elem_t merch_key = {.str_value = current_order->merch_name};
	  if (!merch_exists(db, merch_key) || current_order->quantity > get_quantity_for_merch(db, merch_key))
	    {
	      ioopm_iterator_destroy(iter);
	      return false;
	    }
	}
      ioopm_iterator_destroy(iter);
    
      return true;
    }
  return false;
}

static void checkout_merch(ioopm_db_t *db, char *merch_name, int quantity)
{ 
  elem_t merch_key = {.str_value = merch_name};
  elem_t merch_info = get_value_for_merch(db, merch_key);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(merch_info.merch_value->shelf);
  while (ioopm_iterator_has_next(iter))
    {
      ioopm_iterator_next(iter);
      ioopm_shelf_t *shelf = ioopm_iterator_current(iter).shelf_value;
      int shelf_stock = shelf->quantity;
      if (shelf_stock >= quantity)
	{
	  shelf->quantity = shelf_stock - quantity;
	  break;
	}
      else
	{
	  shelf->quantity = 0;
	  quantity = quantity - shelf_stock;
	}
    }
  ioopm_iterator_destroy(iter);
}

void ioopm_checkout(ioopm_db_t *db, int cart_id)
{
  elem_t cart;
  elem_t *cart_address = &cart;
  if(find_cart(db, cart_id, cart_address))
    {
      ioopm_list_iterator_t *iter = ioopm_list_iterator(cart_address->cart_value->order);
      while(ioopm_iterator_has_next(iter))
	{
	  ioopm_iterator_next(iter);
	  ioopm_order_t *order = ioopm_iterator_current(iter).order_value;
	  checkout_merch(db, order->merch_name, order->quantity);
	}
      ioopm_iterator_destroy(iter);
    }
  ioopm_remove_cart(db, cart_id);
}

#endif










