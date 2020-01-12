#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

// --------------------------
// DEFINES

#define LOAD_FACTOR_MAX 0.75
#define NR_BUCKETS_INITIAL 17

typedef struct entry entry_t;
typedef bool(*bucket_pred)(ioopm_hash_table_t *ht, entry_t *cursor, elem_t element);

// --------------------------
// STRUCTS

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  size_t nr_of_buckets;
  size_t nr_of_entries;
  ioopm_hash_function hash_func;
  ioopm_eq_function key_eq_func; //for comparing key types
  ioopm_eq_function value_eq_func; //for comparing value types
  entry_t *buckets;
};


// --------------------------
// STATIC FUNCTIONS

static bool key_in_bucket(ioopm_hash_table_t *ht, entry_t *cursor, elem_t key)
{
  while(cursor)
  {
    if(ht->key_eq_func(cursor->key, key))
      return true;
    cursor = cursor->next;
  }
  return false;
}

static bool value_in_bucket(ioopm_hash_table_t *ht, entry_t *cursor, elem_t value)
{
  while(cursor)
  {
    if(ht->value_eq_func(cursor->value, value))
      return true;
    cursor = cursor->next;
  }
  return false;
}

static bool has_aux(ioopm_hash_table_t *ht, elem_t element, bucket_pred predicate)
{
  bool pred_result = false;
  entry_t *cursor;
  entry_t *dummy;

  for(size_t i = 0; i < ht->nr_of_buckets && !pred_result; ++i)
  {
    dummy = &ht->buckets[i];
    cursor = dummy->next;

    pred_result = predicate(ht, cursor, element);
  }
  return pred_result;
}

//Returns the element in the bucket ahead of the element we want to create
static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *cursor, elem_t key)
{
  if(!cursor)
    return NULL;
  
  entry_t *next = cursor->next;

  //Iterate the entry chain until the next entry is NULL or next key is equal to key
  while(next)
  {
    if((ht->key_eq_func(next->key, key)))
      return cursor;
    cursor = next;
    next = next->next;
  }
  
  return cursor;
}


static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  //add values to new entry and allocate space for it
  entry_t *new_entry = calloc(1, sizeof(entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;
  
  return new_entry;
}


//Deallocates an entry from the heap
static void entry_destroy(entry_t *entry)
{
  free(entry);
}

static void clear_entry_chain(entry_t *cursor)
{
  if(cursor)
  {
    entry_t *next = cursor->next;
    free(cursor);
    clear_entry_chain(next);
  }
}

static size_t get_bucket_index(ioopm_hash_table_t *ht, elem_t key)
{
  unsigned long int_key;
  
  if(ht->hash_func == NULL)
  {
    int_key = key.int_value;
  }
  else
  {
    int_key = ht->hash_func(key);
  }
  return (int_key % ht->nr_of_buckets);
}

static float load_factor(ioopm_hash_table_t *ht)
{
  return (float) ht->nr_of_entries / (float) ht->nr_of_buckets;
}

static bool should_resize(ioopm_hash_table_t *ht)
{
  if(load_factor(ht) > LOAD_FACTOR_MAX)
    return true;
  else
    return false;
}


//Get the next size from the prime table
static size_t get_next_table_size(ioopm_hash_table_t *ht)
{
  //prime_table is an array of prime numbers used for the size of the hash table
  //Declaring this static allows us to avoid allocating this continuously
  //It will be initialized once, and then remain in memory until the program exits
  static const size_t prime_table[] = { 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869 };
  size_t size_candidate = ht->nr_of_buckets;

  for(size_t i = 0; i < sizeof(prime_table); ++i)
  {
    if(prime_table[i] > size_candidate)
    {
      size_candidate = prime_table[i];
      break;
    }
  }
  return size_candidate;
}

//Resize the hash table
static void hash_table_resize(ioopm_hash_table_t *ht)
{
  //Cache the old buckets
  entry_t *old_buckets = ht->buckets;
  size_t new_size = get_next_table_size(ht);
  size_t old_size = ht->nr_of_buckets;

  ht->buckets = calloc(new_size, sizeof(entry_t));
  assert(ht->buckets);
  ht->nr_of_buckets = new_size;
  ht->nr_of_entries = 0;

  entry_t *cursor;
  entry_t *to_remove;
  
  //Iterate through the old buckets
  for(size_t i = 0; i < old_size; ++i)
  {
    cursor = old_buckets[i].next;
    
    //While there are entries left in the chain
    while(cursor)
    {
      to_remove = cursor;
      ioopm_hash_table_insert(ht, cursor->key, cursor->value);
      cursor = cursor->next;
      entry_destroy(to_remove); //destroy the entries as we iterate through them
    }
  }
  free(old_buckets);
}

// -----------------
// PUBLIC FUNCTIONS

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  entry_t *dummy_node = NULL;
  entry_t *cursor = NULL;
  
  for(size_t i = 0; i < ht->nr_of_buckets; ++i)
  {
    dummy_node = &ht->buckets[i];
    cursor = dummy_node->next;
    clear_entry_chain(cursor);
    dummy_node->next = NULL;
  }
  ht->nr_of_entries = 0;
}


ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_func, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  assert(key_eq_func);
  assert(value_eq_func);
  
  // Allocate and initialize the hash table
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->nr_of_buckets = NR_BUCKETS_INITIAL;
  result->buckets = calloc(NR_BUCKETS_INITIAL, sizeof(entry_t));
  result->hash_func = hash_func;
  result->key_eq_func = key_eq_func;
  result->value_eq_func = value_eq_func;
  
  return result;
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  /// Find the previous entry for key
  size_t bucket = get_bucket_index(ht, key);
  entry_t *previous = find_previous_entry_for_key(ht, &(ht->buckets[bucket]), key);
  entry_t *next = previous->next;

  if(next)
    return Success(next->value);
  else
    return Failure();
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  if(ht)
  {
    // Calculate the bucket for this entry
    size_t bucket = get_bucket_index(ht, key);
    // Search for an existing entry for a key
    entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
    entry_t *next = entry->next;

    // Check if the next entry should be updated or not
    if (next && ht->key_eq_func(next->key, key))
    {
      next->value = value;
    }
    else
    {    
      entry->next = entry_create(key, value, next);
      ht->nr_of_entries++;
    }
    if(should_resize(ht))
      hash_table_resize(ht);
  }
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  if(ht)
  {
    entry_t *previous;
    entry_t *entry_to_remove;
    elem_t removed_value;
    size_t bucket = get_bucket_index(ht, key);

    previous = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
    if(previous->next && ht->key_eq_func(previous->next->key, key))
    {
      removed_value = previous->next->value;
	
      //Link the entries between entry_to_remove
      entry_to_remove = previous->next;
      previous->next = previous->next->next;

      entry_destroy(entry_to_remove);
      ht->nr_of_entries--;
      return Success(removed_value);   
    }
  }
  return Failure();
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  if(ht)
  {
    ioopm_hash_table_clear(ht);
    free(ht->buckets);
    free(ht);
  }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  assert(ht);
  return ht->nr_of_entries;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  assert(ht);
  return ht->nr_of_entries == 0;
}


ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  if(ht)
  {
    ioopm_list_t *keys = ioopm_linked_list_create(ht->key_eq_func);
  
    entry_t *dummy_node, *next;
    
    for(size_t i = 0; i < ht->nr_of_buckets; ++i)
    {
      dummy_node = &ht->buckets[i];
      next = dummy_node->next;

      //While there exists entries in the entry chain
      while(next)
      {
	ioopm_linked_list_append(keys, next->key);
	next = next->next;
      }
    }
    return keys;
  }
  else
    return NULL;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  if(ht)
  {
    ioopm_list_t *values = ioopm_linked_list_create(ht->value_eq_func);

    entry_t *dummy_node, *next;

    for(size_t i = 0; i < ht->nr_of_buckets; ++i)
    {
      dummy_node = &ht->buckets[i];
      next = dummy_node->next;
      
      while(next)
      {
	ioopm_linked_list_append(values, next->value);
	next = next->next;
      }
    }
    return values;
  }
  else
    return NULL;
}
    

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  assert(ht);

  return has_aux(ht, key, key_in_bucket);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  assert(ht);
  
  return has_aux(ht, value, value_in_bucket);
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_hash_predicate pred, void *extra)
{
  assert(ht);
  assert(pred);
  
  entry_t *cursor;
  entry_t *dummy;

  for(size_t i = 0; i < ht->nr_of_buckets; i++)
  {
    dummy = &ht->buckets[i];
    cursor = dummy->next;
    
    while(cursor)
    {
      if(!pred(cursor->key, cursor->value, extra))
	return false;
      cursor = cursor->next;
    }
  }
  return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_hash_predicate pred, void *extra)
{
  assert(ht);
  assert(pred);
  
  bool result = false;
  entry_t *cursor;
  entry_t *dummy;
  
  for(size_t i = 0; i < ht->nr_of_buckets; i++)
  {
    dummy = &ht->buckets[i];
    cursor = dummy->next;
    
    while(cursor)
    {
      if(pred(cursor->key, cursor->value, extra))
	return true;
      cursor = cursor->next;
    }
  }
  
  return result;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_hash_apply_function apply_func, void *extra)
{
  if(ht && apply_func)
  {
    entry_t *dummy, *cursor;
    
    for(size_t i = 0; i < ht->nr_of_buckets; i++)
    {
      dummy = &ht->buckets[i];
      cursor = dummy->next;
    
      while(cursor)
      {
	apply_func(cursor->key, &cursor->value, extra);
	cursor = cursor->next;
      }
    }
  }
}
