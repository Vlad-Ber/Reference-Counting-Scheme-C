#ifndef _FRONTENDH_
#define _FRONTENDH_

#include <stdlib.h>
#include <stdbool.h>
#include "common.h"

/**
 * @file frontend.h
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

/// @brief Shows the stock for the database.
/// @param database, the database operated upon.
void ioopm_show_stock(ioopm_db_t *database);

/// @brief Lists the existing merch in the database.
/// @param database, the database operated upon.
void ioopm_list_merch_UI(ioopm_db_t *database);

/// @brief Lists the existing carts in the database.
/// @param database, the database operated upon.
void ioopm_list_carts(ioopm_db_t *database);

#endif
