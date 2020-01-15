#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H

#include "hash_table.h"


/*
 * @file user_interface.h
 * @author Simon Sandbom, Vladislav Bertilsson
 * @date 8 okt 2019
 * @brief User interface module for inventory manager
*/

/// @brief Prints the menu for the user and is the running loop of the program
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name
/// @param list_of_carts is the linked list storing all carts
void inventory_manager_menu(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, ioopm_list_t *list_of_carts);

/// @brief Adds a merchandise to the warehouse db, including asking user for fields of merch
/// @param warehouse hash table used to store merch
void warehouse_add_merchandise(ioopm_hash_table_t *warehouse);

/// @brief Prints all existing merchandise in the warehouse
/// @param warehouse hash table used to store merch
/// @return Returns a list of all keys from warehouse in order that is displayed by print
char **warehouse_list_all_merch(ioopm_hash_table_t *warehouse);

/// @brief Removes an entry from the warehouse, user chooses which one
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name based on shelf
void warehouse_remove(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, ioopm_list_t *list_of_carts);

/// @brief Edits an existing merchandise based on user choice
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name based on shelf
void warehouse_edit_merchandise(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db, ioopm_list_t *list_of_carts);

/// @brief Shows the stock for a merchandise chosen by user
/// @param warehouse hash table used to store merch
void warehouse_show_stock(ioopm_hash_table_t *warehouse);

/// @brief Increases stock of a merchandise at new or existing shelf by atleast 1
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name based on shelf
void warehouse_replenish_merchandise(ioopm_hash_table_t *warehouse, ioopm_hash_table_t *shelf_db);

/// @brief Creates a cart and adds it to the list of carts
/// @param a list of all carts
void warheouse_create_cart(ioopm_list_t *list_of_carts);

/// @brief Removes a cart from the list of carts
/// @param a list of carts
void warehouse_remove_cart(ioopm_list_t *list_of_carts);

/// @brief Adds a merchandise to a specific cart
/// @param warehouse hash table used to store merch
/// @param list_of_carts linked list of carts
void warehouse_add_to_cart(ioopm_hash_table_t *warehouse, ioopm_list_t *list_of_carts);

/// @brief removes an item from a cart
/// @param list_of_carts linked list of carts
void warehouse_remove_from_cart(ioopm_list_t *list_of_carts);

/// @brief lists carts and asks user to pick one
/// @param list_of_carts linked list of carts
int list_cart_items(ioopm_list_t *list_of_carts);

/// @brief calculates cost of a certain cart based on user choice
/// @param list_of_carts linked list of carts
void warehouse_calculate_cost(ioopm_list_t *list_of_carts);

/// @brief checksout a cart based on user choice
/// @param warehouse hash table used to store merch
/// @param shelf_db hash table used to store merch name based on shelf
void warehouse_checkout_cart(ioopm_hash_table_t *warehouse, ioopm_list_t *list_of_carts, ioopm_hash_table_t *shelf_db);

#endif
