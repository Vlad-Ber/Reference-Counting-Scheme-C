#include <stdio.h>
#include "user_interface.h"
#include "business_logic.h"
#include "hash_table.h"

// Runs the program
int main()
{
  //Creates the databases
  ioopm_hash_table_t *warehouse = warehouse_create();
  ioopm_hash_table_t *shelf_db = shelf_db_create();
  ioopm_list_t *list_of_carts = create_list_of_carts();
  //Prints menu & runs loop
  inventory_manager_menu(warehouse, shelf_db, list_of_carts);
  return 0;
}
