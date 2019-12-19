#pragma once

#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list_linked.h"
#include "common.h"

/**
 * @file hash_table.h
 * @author David Boman, Andreas Harju Schnee
 * @date 16 Sep 2019
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see http://wrigstad.com/ioopm19/assignments/assignment1.html
 */

/// @brief return a pointer to the entry before the entry mapped to by key.
/// @param ht the hash table operated upon.
/// @param key key to lookup in the hashtable.
/// @return a pointer to the previous entry;
entry_t *ioopm_find_previous_entry_for_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief Create a new hash table
/// @param Compare_key function to compare the keys of the hash table.
/// @param Compare_key function to compare the values of the hash table.
/// @param hash_fun function to calculate hashkey.
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function compare_key, ioopm_eq_function compare_value, ioopm_hash_function hash_fun);


/// @brief Create a new hash table, with the user supplied loadfactor and starting number of buckets.
/// @param Compare_key function to compare the keys of the hash table.
/// @param Compare_key function to compare the values of the hash table.
/// @param hash_fun function to calculate hashkey.
/// @param load_factor the loadfactor desides when to rezise the hash_table.
/// @param size_t no_buckets initial number of buckets.
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create_with_load_factor(ioopm_eq_function compare_key, ioopm_eq_function compare_value, ioopm_hash_function hash_fun, float load_factor, size_t no_buckets);


/// @brief add key => value entry in hash table ht, if the key exist the value is replased by the new value.
/// @param ht hash table operated upon
/// @param key key to insert. If int_value of key is less than 0 errno is set to EINVAL. 
/// @param value value to insert, can't be the empty string ("").
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief Delete a hash table and free its memory
/// param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief lookup value for key in hash table ht. sets the parameter value to the value mapt to the key.
/// @param ht hash table operated upon
/// @param key key to lookup. If key < 0 errno is set to EINVAL.
/// @param A pointer to value
/// @return true if key exist in the hashtable else false. Returns NULL if unallowed key.
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return True if all entries in ht satisfies the predicate else false.
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
/// @return True if any entry in ht satisfies the predicate else false.
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_function apply_fun, void *arg);

/// @brief removes the entry mapped to by key and return the value removed.
/// @param key key to remove. If key is less then 0 errno is set to EINVAL.
/// @param h hash table operated upon
/// @return the value mapped to by key. Return -1 if unallowed key.
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table (dummy values excluded)
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return an linked list  of keys for hash table h. uses calloc so the pointer must be freed.
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought. If key is less then 0 errno is set to EINVAL.
/// @return True if key exists in ht, else false. Returns NULL if unallowed key.
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief Checks for equality between two integers.
/// @param a should contain a integer.
/// @param b should contain a integer.
/// @return -1 if a < b, 0 if a == b, 1 if a > b.
int ioopm_eq_function_int(elem_t a, elem_t b);

/// @brief Check for equality between two strings.
/// @param a should contain a string.
/// @param b should contain a string.
/// @return 0 if the strings have the same length and contains the same word, else not 0.
int ioopm_eq_function_str(elem_t a, elem_t b);

