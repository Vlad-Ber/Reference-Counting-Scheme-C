#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"
#include "user_interface.h"
#include "business_logic.h"
#include "hash_table.h"
#include "linked_list.h"

#define max_merch_display 20

/*
*****  STATIC FUNCTIONS   *****
*/

static void print_header()
{
  printf("                                                                                                         \
                                                                                                       \n\
8 odYo. o    o .oPYo. odYo.  o8P .oPYo. oPYo. o    o   ooYoYo. .oPYo. odYo. .oPYo. .oPYo. .oPYo. oPYo. \n\
8 8' `8 Y.  .P 8oooo8 8' `8   8  8    8 8  `' 8    8   8' 8  8 .oooo8 8' `8 .oooo8 8    8 8oooo8 8  `' \n\
8 8   8 `b..d' 8.     8   8   8  8    8 8     8    8   8  8  8 8    8 8   8 8    8 8    8 8.     8     \n\
8 8   8  `YP'  `Yooo' 8   8   8  `YooP' 8     `YooP8   8  8  8 `YooP8 8   8 `YooP8 `YooP8 `Yooo' 8     \n\
....::..::...:::.....:..::..::..::.....:..:::::....8 ::..:..:..:.....:..::..:.....::....8 :.....:..::::\n\
::::::::::::::::::::::::::::::::::::::::::::::::ooP'.::::::::::::::::::::::::::::::::ooP'.:::::::::::::\n\
::::::::::::::::::::::::::::::::::::::::::::::::...::::::::::::::::::::::::::::::::::...:::::::::::::::\n"); 
}

//Prints all the options the user can make
static void print_menu()
{
  char *list_of_options[] = {
    "[1] Add merchandise",
    "[2] Listing merchandise",
    "[3] Remove merchandise",
    "[4] Edit merchandise",
    "[5] Show stock",
    "[6] Replentish",
    "[7] Create Cart",
    "[8] Remove Cart",
    "[9] Add to Cart",
    "[10] Remove from Cart",
    "[11] Calculate cost",
    "[12] Checkout",
    "[13] Undo",
    "[14] Quit"
  };

  for(int i = 0; i < 14 ; ++i)
    {
      printf("%s \n", list_of_options[i]);
    } 
}

static bool name_available(ioopm_hash_table_t *warehouse, char *name)
{
  elem_t name_check;
  name_check.str_value = name;
  if(ioopm_hash_table_has_key(warehouse, name_check))
    {
      printf("There already exists a merch with the name you are trying to enter\n");
      free(name);
      return false;
    }
  else
    {
      return true;
    }
}

static char ask_question_char(char *question)
{
	char user_char;
	do
    {
		char *input = ask_question_string(question);
		user_char = toupper(input[0]);
		free(input);
	}
  while(65 > user_char && 90 < user_char);
	
	return user_char;
}


static char **sort_merch_names(char **unsorted_keys, size_t size_of_array)
{
  char *temp;
  for(size_t i = 0; i < size_of_array; i++)
    {
      for(size_t j = i+1; j < size_of_array; j++)
        {
          if(strcmp(unsorted_keys[i],unsorted_keys[j])>0)
            {
              temp = unsorted_keys[i];
              unsorted_keys[i] = unsorted_keys[j];
              unsorted_keys[j] = temp;
            }
        }
    }
  return unsorted_keys;
}


static bool go_next_page()
{

  bool running = true;

  do
    {
    char user_choice = ask_question_char("Do you wish to go to the next page? ['C'] Yes, ['S'] Quit\n");
    switch(user_choice)
      {
      case('C'):
        return true;
        break;
      case('S'):
        return false;
        break;
      default:
        //printf("Press ['C'] to continue to the next page or ['S'] to stop browsing \n");
        break;
      }
  }

  while(running);
  return false;
}


static char **print_names_of_merch(ioopm_list_t *list_of_values, size_t nr_of_values)
{
  char **stored_keys = calloc(nr_of_values, sizeof(char *));

  int count = 0;
  for(size_t i = 0; i < nr_of_values; ++i)
    {
      stored_keys[i] = ioopm_linked_list_get(list_of_values, i).element.str_value;
    }
  char **sort_keys = sort_merch_names(stored_keys, nr_of_values);

  printf("The following merchandise is currently stored in the warehouse:\n");
  
  for(size_t i = 0; i < nr_of_values; i++)
    {
      printf("[%d] Name: %s\n", (int)i+1, sort_keys[i]);
      printf("\n");
      ++count;
      if(count == max_merch_display && count < (int)nr_of_values-1)
        {
          if(go_next_page())
            {
              count = 0;
            }
          else
            {
              break;
            }
        }

      }
  ioopm_linked_list_destroy(list_of_values);
  
  return sort_keys;
}


static char *ask_question_shelf(char *question)
{
  char *user_input;
  bool right_format_shelf = false;

  do
    {
    user_input = (ask_question(question, not_empty, (convert_func) strdup).string_value);
    if(strlen(user_input) == 3 && isalpha(user_input[0]) && isdigit(user_input[1]) && isdigit(user_input[2]))
      {
      right_format_shelf = true;
      user_input[0] = toupper(user_input[0]);
      
      return user_input;
      }
    else
      {
      printf("The shelf format is wrong, shelf input needs to be on the format |CHAR|INT|INT,example: A25, your format was %s\n", user_input);
    }
  }
  while(!right_format_shelf);
  return "";
}

static bool is_valid_index(int index, int upper_bound)
{
  if(index > 0 && index <= upper_bound)
    {
      return true;
    }
  else
    {
      printf("The index does not correlate to an alternative presented\n");
      return false;
    }
}

static void print_carts(ioopm_list_t *list_of_carts)
{
  for(size_t i = 0; i < ioopm_linked_list_size(list_of_carts); ++i)
    {
      int cart_index = get_cart_index(list_of_carts, i);
      printf("[%d] Cart with index: %d\n", (int) i+1, cart_index);
    }
}

/*
*****   PUBLIC FUNCTION   *****
*/
void warehouse_add_merchandise(ioopm_hash_table_t *warehouse)
{
  //Ask user for name, desc and price inputs that user decides
  char *name = ask_question_string("What is the name of the merch?");
  if(!name_available(warehouse, name))
  {
    return;
  }
  
  char *description = ask_question_string("What is the description of the merch?");
  int price = ask_question_int("What is the price of the merch?");

  if(price <= 0)
    {
      printf("The price of the ware must be above 0 - merch was not added \n");
      free(name);
      free(description);
      
      return;
    }

  warehouse_add_merch_with_fields(warehouse, name, description, price, NULL);
  
  printf("Added the following item to the warehouse: \n \
Name: %s \n Description: %s \n Price: %d \n", name, description, price);
}

//TOOD: Support listing multiple pages, where each page has at most 20 merch
char **warehouse_list_all_merch(ioopm_hash_table_t *warehouse)
{
  //Retrieves a linked list of all keys (names) in ht
  ioopm_list_t *list_of_values = ioopm_hash_table_keys(warehouse);
  //Nr of entered values
  size_t nr_of_values = ioopm_linked_list_size(list_of_values);

  //Dont list items if the warehouse is empty
  if(nr_of_values > 0)
    {
      char **stored_keys = print_names_of_merch(list_of_values, nr_of_values);
      return stored_keys;
    }
  
  else
    {
      printf("The warehouse is empty, try adding an item before using this function\n");
      ioopm_linked_list_destroy(list_of_values);
      
      return NULL;
    }
  
  ioopm_linked_list_destroy(list_of_values);
  
  return NULL;
}


void warehouse_remove_merch(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *db_shelf, ioopm_list_t *list_of_carts)
{
  char **stored_keys = warehouse_list_all_merch(warehouse);

  //If the database is empty, then dont perform the remove function
  if(!stored_keys)
    {
      return;
    }
  
  int index_to_remove = ask_question_int("Press the number of \
the item you want to remove");

  if(!is_valid_index(index_to_remove, ioopm_hash_table_size(warehouse)))
    {
      free(stored_keys);
      return;
    }
  
  elem_t key_to_remove;
  key_to_remove.str_value = stored_keys[index_to_remove-1];

  warehouse_remove_merch_with_key(warehouse, db_shelf, key_to_remove, list_of_carts);
  free(stored_keys);
}


void warehouse_edit_merchandise(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, ioopm_list_t *list_of_carts)
{
  //All keys stored in warehouse
  char **stored_keys = warehouse_list_all_merch(warehouse);

  if(!stored_keys)
    {
      return;
    }
  
  int index_to_remove = ask_question_int("Press the number of \
the merchandise you want to edit");

  if(!is_valid_index(index_to_remove, ioopm_hash_table_size(warehouse)))
    {
      free(stored_keys);
      return;
    }

  elem_t key_to_edit;
  key_to_edit.str_value = stored_keys[index_to_remove-1];

  if(is_merch_in_any_cart(list_of_carts, key_to_edit.str_value))
    {
      free(stored_keys);
      return;
    }

  //Init user values
  char *name = ask_question_string("What is the new name of the merch?");
  if(!name_available(warehouse, name))
  {
    free(stored_keys);
    
    return;
  }
  char *description = ask_question_string("What is the new description of the merch?");
  int price = ask_question_int("What is the new price of the merch?");

  
  if(price <= 0)
    {
      printf("The price of the ware must be above 0");
      free(stored_keys);
      
      return;
    }

  warehouse_edit_merchandise_with_fields(warehouse, shelf_db, name, description, price, key_to_edit, list_of_carts);

  free(stored_keys);
}

void warehouse_show_stock(ioopm_hash_table_t *warehouse)
{
  if(ioopm_hash_table_is_empty(warehouse))
    {
      printf("The warehouse is empty, try adding a merchandise to the warehouse first!\n");
      
      return;
    }
  char **stored_keys = warehouse_list_all_merch(warehouse);

  int index_to_show_stock = ask_question_int("Press the number of the merchandise you want to show the stock of");

  if(!is_valid_index(index_to_show_stock, ioopm_hash_table_size(warehouse)))
    {
      free(stored_keys);
      
      return;
    }
  
  elem_t merch_to_show_stock_key;
  merch_to_show_stock_key.str_value = stored_keys[index_to_show_stock-1];

  if(warehouse_key_has_locations(warehouse, merch_to_show_stock_key))
    {
    warehouse_list_locations(warehouse, merch_to_show_stock_key);
  }
  else
    {
      printf("This merchandise is currently not stored anywhere, to store it use the Replenish function in the main menu\n");
    }
  free(stored_keys);
}


void warehouse_replenish_merchandise(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db)
{
  if(ioopm_hash_table_is_empty(warehouse))
    {
      printf("The warehouse is empty, try adding a merchandise to the warehouse first!\n");
      
      return;
    }
  char **stored_keys = warehouse_list_all_merch(warehouse);

  int index_to_replenish = ask_question_int("Press the number of the merchandise \
you want to replentish");

  if(!is_valid_index(index_to_replenish, ioopm_hash_table_size(warehouse)))
    {
      free(stored_keys);
      
      return;
    }

  elem_t merch_to_replenish_key;
  merch_to_replenish_key.str_value = stored_keys[index_to_replenish-1];

  if(warehouse_key_has_locations(warehouse, merch_to_replenish_key))
    {
      printf("The merch you chose is stored at the following locations:\n");
      warehouse_list_locations(warehouse, merch_to_replenish_key);
      
      char *shelf = ask_question_shelf("Enter a location to store the merchandise at (entering a location that the merch is already on will instead only increase the amount)");
      if(is_shelf_available(warehouse, shelf_db, shelf, merch_to_replenish_key))
        {
        int amount = ask_question_int("How many do you want to store at this location?");
        
        if(amount <= 0)
          {
            printf("You cannot store 0 or a negative amount\n");
            free(stored_keys);
            
            return;
          }

        warehouse_store_location(warehouse, shelf_db, shelf, merch_to_replenish_key, amount);
      }
      free(stored_keys);
      
      return;
    }
   else
    {
      printf("This merch is not stored at any location yet\n");
      char *shelf = ask_question_shelf("Enter a location to store the merchandise at");
      if(is_shelf_available(warehouse, shelf_db, shelf, merch_to_replenish_key)){
        int amount = ask_question_int("How many do you want to store at this location?");

        if(amount <= 0)
          {
            printf("You cannot store 0 or a negative amount\n");
            free(shelf);
            free(stored_keys);
            
            return;
          }
        warehouse_store_location(warehouse, shelf_db, shelf, merch_to_replenish_key, amount);
      }
    }
      free(stored_keys);
      
      return;
}

void warehouse_create_cart(ioopm_list_t *list_of_carts)
{
  warehouse_add_cart(list_of_carts);
}

void warehouse_remove_cart(ioopm_list_t *list_of_carts)
{
  if(ioopm_linked_list_is_empty(list_of_carts))
    {
      printf("There are currently no carts, try creating a cart first!\n");
      return;
    }
  print_carts(list_of_carts);
  
  int user_index_to_remove = ask_question_int("Choose a cart to remove\n");

  if(!is_valid_index(user_index_to_remove, ioopm_linked_list_size(list_of_carts)))
    {
      return;
    }
  warehouse_delete_cart_with_index(list_of_carts, user_index_to_remove-1);
}


void warehouse_add_to_cart(ioopm_hash_table_t *warehouse, ioopm_list_t *list_of_carts)
{
  if(ioopm_linked_list_is_empty(list_of_carts))
    {
      printf("There are currently no carts, try creating a cart first\n");
      return;
    }
  print_carts(list_of_carts);
  
  int user_cart_to_add_to = ask_question_int("Choose a cart to add a merchandise to\n");
  
  if(!is_valid_index(user_cart_to_add_to, ioopm_linked_list_size(list_of_carts)))
    {
      return;
    }

  char **available_merch = warehouse_list_all_merch(warehouse);

  if(!available_merch)
    {
      return;
    }

  int merch_to_add = ask_question_int("Which merchandise would you like to add?");

  if(!is_valid_index(merch_to_add, ioopm_hash_table_size(warehouse)))
    {
      return;
    }
    
  char *name_of_merch = available_merch[merch_to_add-1];
  elem_t key_of_merch;
  key_of_merch.str_value = name_of_merch;

  int amount_to_add = ask_question_int("How many would you like to add?");

  if(amount_to_add <= 0)
    {
      printf("You cannot store 0 or a negative amount\n");
      free(available_merch);
      return;
    }
  
  if(!is_merch_stocked(warehouse, key_of_merch) || !amount_to_add_available(warehouse, key_of_merch, amount_to_add))
    {
      free(available_merch);
      return;
    }
  
  printf("Adding %d of %s to cart %d\n", amount_to_add, name_of_merch, user_cart_to_add_to);

  add_merchandise_to_cart_with_index(warehouse, list_of_carts, user_cart_to_add_to-1, key_of_merch, amount_to_add);

  free(available_merch);
}

void warehouse_remove_from_cart(ioopm_list_t *list_of_carts)
{
  if(ioopm_linked_list_is_empty(list_of_carts))
    {
      printf("There are currently no carts, try creating a cart first\n");
      return;
    }
  
  print_carts(list_of_carts);
  
  int user_cart_to_remove_from = ask_question_int("Choose a cart to remove merchandise from \n");

  if(!is_valid_index(user_cart_to_remove_from, ioopm_linked_list_size(list_of_carts)) || is_cart_empty(list_of_carts, user_cart_to_remove_from-1))
    {
      return;
    }

  warehouse_list_cart_merch(list_of_carts, user_cart_to_remove_from-1);

  int user_merch_to_remove_from = ask_question_int("Choose an item to remove from");

  if(!is_valid_index(user_merch_to_remove_from, warehouse_size_of_cart(list_of_carts, user_merch_to_remove_from-1)))
    {
      return;
    }

  int amount_to_remove = ask_question_int("How many do you want to remove from the cart?");

  if(amount_to_remove <= 0)
    {
      printf("You cannot remove 0 or a negative amount\n");
      return;
    }
  
  remove_cart_merch_with_index(list_of_carts, user_cart_to_remove_from-1, user_merch_to_remove_from-1, amount_to_remove); 
}

int list_cart_items(ioopm_list_t *list_of_carts)
{
  print_carts(list_of_carts);

  int user_cart_to_print = ask_question_int("Choose a cart\n");

  if(!is_valid_index(user_cart_to_print, ioopm_linked_list_size(list_of_carts)))
    {
      printf("The number you entered does not correlate to a cart in the list\n");
      return 0;
    }
  if(is_cart_empty(list_of_carts, user_cart_to_print-1))
    {
      return 0;
    }

  warehouse_list_cart_merch(list_of_carts, user_cart_to_print-1);

  size_t cost = warehouse_total_cost_of_cart_with_index(list_of_carts, user_cart_to_print-1);
  printf("The total cost for these items are: %d\n ", (int) cost);

  return user_cart_to_print;
}

void warehouse_calculate_cost(ioopm_list_t *list_of_carts)
{
  if(ioopm_linked_list_is_empty(list_of_carts))
    {
      printf("There are no carts in the warehouse, try creating one first!\n");
      return;
    }
  
  print_carts(list_of_carts);
  
  int user_cart_to_calculate = ask_question_int("Choose a cart to calculate the cost of\n");
  
  if(!is_valid_index(user_cart_to_calculate, ioopm_linked_list_size(list_of_carts)))
    {
      printf("The number you entered does not correlate to a cart in the list\n");
      return;
    }
  
  size_t cost = warehouse_total_cost_of_cart_with_index(list_of_carts, user_cart_to_calculate-1);

  printf("The total cost of the cart is %d\n", (int) cost);
}


void warehouse_checkout_cart(ioopm_hash_table_t *warehouse, ioopm_list_t *list_of_carts, ioopm_hash_table_t *shelf_db)
{
  if(ioopm_linked_list_is_empty(list_of_carts))
    {
      printf("There are no carts in the warehouse, try creating one first!\n");
      return;
    }
  int cart_nr = list_cart_items(list_of_carts);

  if(cart_nr == 0)
    {
      return;
    }
  /*
  if(warehouse_size_of_cart(list_of_carts, cart_nr-1) <= 0)
    {
      printf("There are current")
    }
  */
  int checkout_confirmation = ask_question_int("Do you want to checkout these merchandise [1] for YES and [2] for NO\n");

  int cart_index = get_cart_index(list_of_carts, cart_nr-1);


  switch(checkout_confirmation)
    {
    case(1):
      printf("Checking out cart %d\n", cart_index);
      warehouse_checkout_cart_aux(warehouse, list_of_carts, cart_nr-1, shelf_db);
      break;
    case(2):
      printf("Going back to main menu, cancelling checkout\n");
      break;
    default:
      printf("Press [1] to checkout or [2] to exit and go back\n");
      break;
    } 
}


void inventory_manager_menu(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db,ioopm_list_t *list_of_carts)
{
  print_header();
  bool  active = true;
  do
    {
      print_menu();
      int user_choice = ask_question_int("Choose an alternative in the menu, \
  by typing the corresponding number (1-14)");
      switch(user_choice)
        {
        case(1):
          warehouse_add_merchandise(warehouse);
          break;
        case(2):
          {
          char **stored_keys;
          stored_keys = warehouse_list_all_merch(warehouse);
          free(stored_keys);
          break;
          }
        case(3):
          warehouse_remove_merch(warehouse, shelf_db, list_of_carts);
          break;
        case(4):
          warehouse_edit_merchandise(warehouse, shelf_db, list_of_carts);
          break;
        case(5):
          warehouse_show_stock(warehouse);
          break;
        case(6):
          warehouse_replenish_merchandise(warehouse, shelf_db);
          break;
        case(7):
          warehouse_create_cart(list_of_carts);
          break;
        case(8):
          warehouse_remove_cart(list_of_carts);
          break;
        case(9):
          warehouse_add_to_cart(warehouse, list_of_carts);
          break;
        case(10):
          warehouse_remove_from_cart(list_of_carts);
          break;
        case(11):
          warehouse_calculate_cost(list_of_carts);
          break;
        case(12):
          warehouse_checkout_cart(warehouse, list_of_carts, shelf_db);
          break;
        case(13):
          printf("Undoing last action\n");
          printf("====THIS FUNCTION IS NOT IMPLEMENTED YET====\n");
          break;
        case(14):
          printf("Quitting program, thanks for using this Inventory manager!\n");
          warehouse_destroy(warehouse, shelf_db, list_of_carts);
          active = false;
          break;
        default:
          printf("Choose one of the alternatives (1-13)\n");
        }
    }
  while(active);
    }
