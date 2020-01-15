#ifndef _HASH_TABLE__H
#define _HASH_TABLE__H
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"
#include "linked_list.h"

#define No_Buckets 17

/**
 * @file hash_table.h
 * @author Christoffer Nyberg, Simon Sandbom
 * @date 4 Okt 2019
 * @brief Simple hash table that maps keys to values.
 *
 * Hash table that maps generic keys to generic values. Any elem_t value can be used as
 * key or value. A hashtable has two parameters that affect its performance,
 * initial capacity and load factor. The inital capacity is the nr of buckets at hash-table
 * creation and load factor is the measure on how full the hash table may get before resizing
 * it and thus increasing it's capacity. The default load factor is generally set as 0.75
 * as higher values decrease the space overhead but increase the time cost to lookup an entry.
 *
 * Note that the hash table is open: In the case of a hash collision, a single bucket can
 * store several entries, which must be searched sequentially.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */

typedef void obj;
typedef struct hash_table ioopm_hash_table_t;
typedef bool(*ioopm_hash_predicate)(elem_t key, elem_t value, void *extra);
typedef void(*ioopm_hash_apply_function)(elem_t key, elem_t *value, void *extra);
typedef size_t(*ioopm_hash_function)(elem_t key);

void doNothing(obj *c);
/// @brief Create a new hash table
/// @param hash_func The hash function to be used on each key
/// @param key_eq_func The function to compare keys for equality
/// @param value_eq_func The function to compare values for equality
/// @pre key_eq_func != NULL, value_eq_func != NULL
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func);

/// @brief Delete a hash table and free its memory
/// Does not free the memory of values stored in the hash table
/// Does nothing if ht == NULL
/// param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add a key, value entry in hash table ht
/// does nothing if ht == NULL
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return a value indicating whether the lookup was succesful, and the key if successful
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return a value indicating whether the remove was successful, and the value removed if successful
option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);


//---------------------
// UTILITY FUNCTIONS
//---------------------

/// @brief returns the number of key-value entries in the hash table in O(1) time
/// @param ht hash table operated upon
/// @pre ht != NULL
/// @return the number of key-value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty in O(1) time
/// @param ht hash table operated upon
/// @pre ht != NULL
/// @return true if the hash table is empty, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// does nothing if ht == NULL
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return a linked list of keys for hash table ht, or NULL if ht is NULL
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash table (in no particular order, but same as ioopm_hash_table_keys)
/// @param ht hash table operated upon
/// @return a linked list of values for hash table ht, or NULL if ht is NULL
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
/// @pre ht != NULL
/// @return true if the key exists in the hash table, otherwise false
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
/// @pre ht != NULL
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @pre ht != NULL, pred != NULL
/// @return true if all entries satisfy the predicate, otherwise false
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_hash_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @pre ht != NULL, pred != NULL
/// @return true if any entry satisfies the predicate, otherwise false
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_hash_predicate pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// does nothing if either ht or apply_fun is null
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_hash_apply_function apply_fun, void *arg);

#endif
