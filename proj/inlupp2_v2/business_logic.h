#ifndef BUSINESS_LOGIC_H
#define BUSINESS_LOGIC_H

#include "hash_table.h"


/**
 * @file business_logic.h
 * @author Simon Sandbom, Vladislav Bertilsson
 * @date 9 okt 2019
 * @brief All non-user interface logic that concerns the warehouse program,
 * e.g. shopping carts, merchandise, adding and removing items from the “database”

 * Naming standard used is that public functions start with warehouse followed by
 * what action they perform, such as warehouse_add_item that adds an item to the
 * warehouse
*/

typedef struct merch merch_t;
typedef struct stock stock_t;
typedef struct cart cart_t;
typedef struct cart_item cart_item_t;

/// @brief Initializes the hash table that stores merch
/// @return Returns the newly created hash table
ioopm_hash_table_t *warehouse_create();

/// @brief Initializes the hash table that stores names of merch
/// @return Returns the newly created hash table
ioopm_hash_table_t *shelf_db_create();

/// @brief Creates a linked list that stores all carts in the warehouse
/// @return returns the created linked list
ioopm_list_t *create_list_of_carts();

/// @brief Adds a merch item to the hash table databases
/// @param warehouse hash table used to store merch
/// @param name is name of merch to add
/// @param description is description of merch
/// @param price is price of merch
/// @param linked list of locations is all locations merch is stored at
void warehouse_add_merch_with_fields(ioopm_hash_table_t *warehouse, char *name, char *description, int price, ioopm_list_t *locations);

/// @brief Removes an existing entry from the hash tables
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name
/// @param key_to_remove, key to merch to be removed
/// @return Return the name of the merchandise removed
void warehouse_remove_merch_with_key(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *db_shelf, elem_t key_to_remove, ioopm_list_t *list_of_carts);

/// @brief Edits an existing merchandises name, description and price
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name
/// @param name is name of merch to add
/// @param description is description of merch
/// @param price is price of merch
/// @param key_to_edit is the key to the merch we want to edit
void warehouse_edit_merchandise_with_fields(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, char *name, char *description, int price, elem_t key_to_edit, ioopm_list_t *list_of_carts);

/// @brief Tears down all hash tables and linked lists and frees all allocated memor
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name
/// @param list_of_carts linked list of all carts
void warehouse_destroy(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, ioopm_list_t *list_of_carts);

/// @brief Checks if a certain merchandise is already stored at any locations
/// @param warehouse hash table used to store merch
/// @param key is key to merch in warehouse
/// @return true if merch is stored anywhere, false otherwise
bool warehouse_key_has_locations(ioopm_hash_table_t *warehouse, elem_t key);

/// @brief stores a merchandise at a shelf as well as stores amount on that shelf
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name
/// @param shelf is shelf to store merch at
/// @param name_of_merch is key to find merch to store
/// @param amount is amount to store on shelf
void warehouse_store_location(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, char *shelf, elem_t name_of_merch, int amount);

/// @brief Lists all locations of a certain merchandise
/// @param warehouse hash table used to store merch
/// @param key to merch, to find locations of merch
void warehouse_list_locations(ioopm_hash_table_t *warehouse, elem_t key);

/// @brief Checks if a certain shelf is available for a certain merch
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name
/// @param shelf to check if available
/// @param key to merch to check locations
/// @return true if available false otherwise
bool is_shelf_available(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, char *shelf, elem_t key);

/// @brief adds a cart to the list of carts linked list
/// @param linked list of all carts
void warehouse_add_cart(ioopm_list_t *list_of_carts);

/// @brief Deletes a cart at index
/// @param linked list of all carts
/// @param index to remove cart at
void warehouse_delete_cart_with_index(ioopm_list_t *list_of_carts, int index);

/// @brief Gets index of cart at index(in list)
/// @param linked list of all carts
/// @param index in list of carts
/// @return returns the index value of a cart
int get_cart_index(ioopm_list_t *list_of_carts, int index);

/// @brief Desroys all carts in the list of carts linked list
/// @param linked list of all carts
void warehouse_destroy_carts(ioopm_list_t *list_of_carts);

/// @brief Adds merchandise to a cart with index of user choice
/// @param warehouse hash table used to store merch
/// @param list_of_carts linked list of all carts
/// @param index of cart to add cart to
/// @param name_of_merch, name of merch to add to cart
/// @param user_amount amount to add
void add_merchandise_to_cart_with_index(ioopm_hash_table_t *warehouse, ioopm_list_t *list_of_carts, int index, elem_t name_of_merch, int user_amount);

/// @brief Lists all merch stored in a specific cart
/// @param list_of_carts linked list of all carts
/// @param index of cart
void warehouse_list_cart_merch(ioopm_list_t *list_of_carts, int index);

/// @brief removes merch from a specific cart
/// @param list_of_carts linked list of all carts
/// @param cart_index index of cart to remove merch from
/// @param merch_index index of merch to remove from cart
/// @param amount_to_remove, amount of merch to remove from cart
void remove_cart_merch_with_index(ioopm_list_t *list_of_carts, int cart_index, int merch_index, int amount_to_remove);

/// @brief Calculates the total cost of all merch in a cart
/// @param list_of_carts, linked list of all carts
/// @param index of cart to calculate merch in
/// @return the sum of all merch's price in cart
size_t warehouse_total_cost_of_cart_with_index(ioopm_list_t *list_of_carts, int index);

/// @brief Checks if there is enough of merch stored for adding to cart
/// @param warehouse hash table used to store merch
/// @param key to merch
/// @param amount_to_add amount to see if it is available
/// @return true if there is enough stored, false otherwise
bool amount_to_add_available(ioopm_hash_table_t *warehouse, elem_t key, int amount_to_add);

/// @brief Checks if the merch is stored somewhere
/// @param warehouse hash table used to store merch
/// @param key to merch
/// @return true if merch is stored somewhere, false otherwise
bool is_merch_stocked(ioopm_hash_table_t *warehouse, elem_t key);

/// @brief Calculates how many different merch is stored in cart
/// @param list_of_carts linked list of all carts
/// @param index of cart to calculate
size_t warehouse_size_of_cart(ioopm_list_t *list_of_carts, int index);

/// @brief deletes the cart and removing amount checked out
/// @param list_of_carts linked list of all carts
/// @param shelf_db hash table storing names based on shelf
void warehouse_checkout_cart_aux(ioopm_hash_table_t *warehouse, ioopm_list_t *list_of_carts, int index, ioopm_hash_table_t *shelf_db);

/// @brief Checks if a cart has merchandise in it
/// @param list_of_carts linked list of all carts
/// @param index of cart to check
/// @return true if cart is empty of merchandise, otherwise false
bool is_cart_empty(ioopm_list_t *list_of_carts, int index);

//AUX FUNCTIONS FOR TESTING

/// @brief returns name of merch in cart
/// @param list_of_carts linked list of all carts
/// @return name of first merch in cart
char *merchCartName(ioopm_list_t *list_of_carts);

/// @brief returns description of merch in warehouse
/// @param warehouse hash table used to store merch
/// @param name of merch
/// @return description of merch with name
char *merch_getdesc(ioopm_hash_table_t *warehouse, char *name);

/// @brief returns amount of merch in warheouse
/// @param warheouse hash table used to store merch
/// @param name of merch
/// @param index of merch
/// @return amount of merch stored
int getAmountSingel(ioopm_hash_table_t *warehouse, char *name, int index);

bool is_merch_in_any_cart(ioopm_list_t *list_of_carts, char *name);



#endif
