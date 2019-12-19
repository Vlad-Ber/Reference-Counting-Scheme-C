#ifndef _BUISNESS_LOGICH_
#define _BUISNESS_LOGICH_

#include <stdlib.h>
#include <stdbool.h>
#include "common.h"

/**
 * @file buisness_logic.h
 * @author Adam Axelsson, Andreas Harju Schnee
 * @date 1 Nov 2019
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */

/// @brief Replenish stock for a merch.
/// @pre no_items should be a positive number.
/// @sideeffect allocates memory for the new shelf if shelf_name is used.
/// @param database the database to be operated upon.
/// @param merch_to_replenish, the name of the merch you want to replenish.
/// @param shelf_to_replenish, the shelf to replenish on.
/// @param no_items, the quantity to replenish.
/// @return True if shelf_to_replenish is used to create a new shelf, else false.
bool ioopm_replenish_stock(ioopm_db_t *database, char *merch_name, char *shelf_name, int no_items);

/// @brief Creates a new cart.
/// @sideeffect allocates memory for the cart.
/// @param database the database to be operated upon.
void ioopm_create_cart(ioopm_db_t *database);

/// @brief Removes a cart from databas.
/// @sideeffect Frees the memory for the cart removed.
/// @param database the database to be operated upon.
/// @param cart_id, the id of the cart to remove.
void ioopm_remove_cart(ioopm_db_t *database, int cart_id);  

/// @brief Edit an existing merch's name, description and price.
/// @sideeffect Frees the memory for the merch edited and allocates memory for the result of the editing.
/// @param database the database operated upon.
/// @param merch_name, the name of the merch to edit.
/// @param new_name, the new name for the merch to edit.
/// @param new_desc, the new description for the merch to edit.
/// @param new_price, the new price for the merch to edit.
/// @return Returns true if new_name and new_desc is used to edit a merch, else false.
bool ioopm_edit_merch(ioopm_db_t *database, char *merch_name, char *new_name, char *new_desc, int new_price);

/// @brief Remove an existing merch, including its stock.
/// @sideeffect Frees the memory for the merch removed.
/// @param database the database to be operated upon.
/// @param merch_name, the name of the merch to remove.
void ioopm_remove_merch(ioopm_db_t *database, char *merch_name);

/// @brief Add a new merch to the database, a newly added merch has no stock.
/// @sideeffect Allocates memory for the new merch.
/// @param database the database to be operated upon.
/// @param name, name for the new merch.
/// @param description, the description for the new merch.
/// @param price, the price for the new merch.
void ioopm_add_merch(ioopm_db_t *database, char *name, char *description, int price);

/// @brief Destroys and frees a database.
/// @sideeffect Frees the memory for the database.
/// @param database the database to be operated upon.
void ioopm_destroy_database(ioopm_db_t *database);

/// @brief Creates a new database.
/// @sideeffect Allocates memory for the new database.
/// @return a pointer to the new database.
ioopm_db_t *ioopm_database_create();

/// @brief Add a new order to an existing cart.
/// @sideeffect Allocates memory for the new order.
/// @param database the database to be operated upon.
/// @param merch_name, the name of the merch for the new order.
/// @param quantity, the number of said merch to add.
/// @param cart_id, the id of the cart to add the order to.
/// @return Returns true if merch_name is used, else false.
bool ioopm_add_to_cart(ioopm_db_t *database, char *merch_name, int quantity, int cart_id);

/// @brief Removes zero or more items of some merch from a particular cart.
/// @param database the database to be operated upon.
/// @param cart_id, the id of the cart to remove from.
/// @param merch_name, the name of the merch to remove from.
/// @param quantity, the quantity to remove.
void ioopm_remove_from_cart(ioopm_db_t *database, int cart_id, char *merch_name, int quantity);

/// @brief Calculate the cost of a given cart.
/// @param database the database to be operated upon.
/// @param cart_id, the id of the cart to calculate cost for.
/// @return The cost of the cart.
int ioopm_calculate_cost(ioopm_db_t *database, int cart_id);

/// @brief Checkout a given cart, and update the stock accordingly.
/// @sideeffect Frees the memory for the cart. 
/// @param database the database to be operated upon.
/// @param cart_id, the id of the cart to checkout.
void ioopm_checkout(ioopm_db_t *database, int cart_id);

/// @brief Check if it's possible to excecute a checkout for a given cart.
/// @param database the database to be operated upon.
/// @param cart_id, the id of the cart to check.
bool ioopm_checkout_possible(ioopm_db_t *db, int cart_id);

/// @brief Creates a new merch
/// @sideeffect Allocates memory for the new merch.
/// @param name, the name of the new merch.
/// @param desc, the description of the new merch.
/// @param price, the price of the new merch.
/// @return Returns the new merch.
ioopm_merch_t *ioopm_create_merch(char *name, char *desc, int price);

#endif
