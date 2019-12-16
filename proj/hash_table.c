#include "hash_table.h"
#include "list_linked.h"
#include "iterator.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int BUCKET_SIZE_ARRAY[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};


static entry_t *entry_create(elem_t key, elem_t value, entry_t *first_entry);
static bool key_equiv(elem_t key, elem_t value_ignored, void *x, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func);
static bool value_equiv(elem_t ignored, elem_t value, void *x, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func);
static void hash_table_insert_into_buckets(entry_t ** buckets,
                                           ioopm_hash_table_t *ht,
                                           elem_t key,
                                           elem_t value);

static unsigned modulo_hash_function(ioopm_hash_table_t *ht, elem_t key)
{
  if (ht->hash_function == NULL)
    {
      return abs(key.i % BUCKET_SIZE_ARRAY[ht->nr_buckets_index]);
    }
  return ht->hash_function(key) % BUCKET_SIZE_ARRAY[ht->nr_buckets_index];
}


ioopm_hash_table_t *ioopm_hash_table_create_load_factor(cmp_fun_t compare_func,
                                                        hash_fun_t hash_function,
                                                        ioopm_eq_function key_eq_function,
                                                        ioopm_eq_function value_eq_function,
                                                        double load_factor)
{
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->size = 0;
  result->compare_func = compare_func;
  result->hash_function = hash_function;
  result->key_eq_function = key_eq_function;
  result->value_eq_function = value_eq_function;
  result->load_factor = load_factor;
  result->nr_buckets_index = 0;
  result->buckets = calloc(BUCKET_SIZE_ARRAY[result->nr_buckets_index],
                           sizeof(entry_t));
  for(int i = 0; i < BUCKET_SIZE_ARRAY[result->nr_buckets_index]; ++i)
    {
      result->buckets[i] = entry_create(int_elem(0), (elem_t){ .p = NULL } , NULL);
    }
  return result;
}



ioopm_hash_table_t *ioopm_hash_table_create(cmp_fun_t compare_func,
                                                        hash_fun_t hash_function,
                                                        ioopm_eq_function key_eq_function,
                                                        ioopm_eq_function value_eq_function)
{
  return ioopm_hash_table_create_load_factor(compare_func, hash_function, key_eq_function,
                                             value_eq_function, 0.50);
}

static void hash_table_extend_capacity(ioopm_hash_table_t *ht)
{

  int new_nr_buckets_index = ht->nr_buckets_index + 1;
  entry_t **new_buckets = calloc(BUCKET_SIZE_ARRAY[new_nr_buckets_index],
                                 sizeof(entry_t));

  for(int i = 0; i < BUCKET_SIZE_ARRAY[new_nr_buckets_index]; ++i)
    {
      new_buckets[i] = entry_create(int_elem(0), (elem_t){.p = NULL}, NULL);
    }
  
  
  ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t *keys = ioopm_list_iterator(keys_list);
  ioopm_list_t *values_list = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t *values = ioopm_list_iterator(values_list);

  ioopm_hash_table_clear(ht);
  for(int i = 0; i < BUCKET_SIZE_ARRAY[ht->nr_buckets_index]; ++i)
    {
      free(ht->buckets[i]);
    }
  free(ht->buckets);

  ht->nr_buckets_index = new_nr_buckets_index;
  while(ioopm_iterator_has_next(keys))
    {
      hash_table_insert_into_buckets(new_buckets,
                                     ht,
                                     *ioopm_iterator_next(keys),
                                     *ioopm_iterator_next(values));
    }
  ht->buckets = new_buckets;
  ioopm_iterator_destroy(keys);
  ioopm_linked_list_destroy(keys_list);
  ioopm_iterator_destroy(values);
  ioopm_linked_list_destroy(values_list);
}

static entry_t *find_previous_entry_for_key(entry_t *entry, elem_t key, cmp_fun_t compare_fun)
{
  entry_t *cursor = entry->next;
  entry_t *previous_entry = entry;

  while (cursor != NULL)
    {
      if (compare_fun(key ,cursor->key))
        {
          return previous_entry;
        }
      else
        {
          previous_entry = cursor;
          cursor = cursor->next;
        }
    }
  return previous_entry;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *first_entry)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = first_entry;
  return new_entry;
}


// Insert into buckets as if it belonged to ht
static void hash_table_insert_into_buckets(entry_t ** buckets,
                                          ioopm_hash_table_t *ht,
                                          elem_t key,
                                          elem_t value)
{
  /// Calculate the bucket for this entry
  int bucket_index = modulo_hash_function(ht, key);
  /// Search for an existing entry for a key
  entry_t *previous_entry = find_previous_entry_for_key(buckets[bucket_index],
                                                        key, ht->compare_func);
  entry_t *next = previous_entry->next;
  if ( next != NULL && ht->key_eq_function(next->key, key))
    {
      next->value = value;
    }
  else
    {
      previous_entry->next = entry_create(key, value, next);
      ht->size++;
    }
  if (ht->size / (double)BUCKET_SIZE_ARRAY[ht->nr_buckets_index] >= ht->load_factor)
    {
      hash_table_extend_capacity(ht);
    }
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht,
                                          elem_t key,
                                          elem_t value)
{
  hash_table_insert_into_buckets(ht->buckets,
                                 ht,
                                 key,
                                 value);
}


bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  entry_t *previous_entry = (find_previous_entry_for_key(ht->buckets[modulo_hash_function(ht, key)],
                                                         key,
                                                         ht->compare_func));
  if (previous_entry->next && ht->key_eq_function(previous_entry->next->key, key))
    {
      *result = previous_entry->next->value;
      return true;
    }
  else
    {
      return false;
    }
}

static void entry_destroy(entry_t **entry)
{
  free(*entry);
  *entry = NULL;
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  entry_t *previous_entry = find_previous_entry_for_key(ht->buckets[modulo_hash_function(ht, key)],
                                                        key,
                                                        ht->compare_func);
  if (previous_entry->next)
    {
      elem_t value_to_remove = previous_entry->next->value;
      entry_t *entry_to_remove = previous_entry->next;
      previous_entry->next = previous_entry->next->next;
      entry_destroy(&entry_to_remove);
      *result = value_to_remove;
      ht->size--;
      return true;
    }
  else
    {
      return false;
    }
}

static void entry_destroy_recursively(entry_t *entry)
{
  entry_t *next = entry;
  while(next!=NULL)
    {
       next = entry->next;
       entry_destroy(&entry);
       entry = next;
    }
  
}


void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i=0; i < BUCKET_SIZE_ARRAY[ht->nr_buckets_index]; i++)
    {
      entry_destroy_recursively(ht->buckets[i]->next);
      ht->buckets[i]->next = NULL;
    }
  ht->size = 0;
}


void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < BUCKET_SIZE_ARRAY[ht->nr_buckets_index]; i++)
    {
      entry_destroy_recursively(ht->buckets[i]);
    }
  free(ht->buckets);
  free(ht);
}
 
int ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return (ht->size == 0);
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ht->key_eq_function);

  for (int i=0; i < BUCKET_SIZE_ARRAY[ht->nr_buckets_index]; i++)
    {
      entry_t *cursor = ht->buckets[i]->next;
      while (cursor != NULL)
      {
        ioopm_linked_list_append(list, cursor->key);
        cursor = cursor->next;
      }
    }
  return list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *all_values = ioopm_linked_list_create(ht->value_eq_function);
  for (int i=0; i < BUCKET_SIZE_ARRAY[ht->nr_buckets_index]; i++)
    {
      entry_t *cursor = ht->buckets[i]->next;
      while (cursor != NULL)
        {
          ioopm_linked_list_append( all_values,  cursor->value);
          cursor = cursor->next;
        }
    }
  return all_values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, key_equiv, &key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, value_equiv, &value);
}
// if b is false this function behaves like all otherwise it behaves like any
static bool hash_table_all_any_helper(bool b, ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t *keys = ioopm_list_iterator(keys_list);
  ioopm_list_t *values_list = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t *values = ioopm_list_iterator(values_list);
  
  for (int i = 0; ioopm_iterator_has_next(keys); ++i)
    {
      if (b == pred(*ioopm_iterator_next(keys),
                    *ioopm_iterator_next(values),
                    arg,
		    ht->key_eq_function,
		    ht->value_eq_function))
        {
          ioopm_iterator_destroy(keys);
          ioopm_linked_list_destroy(keys_list);

          ioopm_iterator_destroy(values);
          ioopm_linked_list_destroy(values_list);
          return b;
        }
    }
  ioopm_iterator_destroy(keys);
  ioopm_linked_list_destroy(keys_list);
  ioopm_iterator_destroy(values);
  ioopm_linked_list_destroy(values_list);
  return !b;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  return hash_table_all_any_helper(false, ht, pred, arg);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  return hash_table_all_any_helper(true, ht, pred, arg);
}

static bool key_equiv(elem_t key, elem_t value_ignored, void *x, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  elem_t *other_key_ptr = x;
  elem_t other_key = *other_key_ptr;
  return key_eq_func(other_key, key);
}

static bool value_equiv(elem_t ignored, elem_t value, void *x, ioopm_eq_function key_eq_func, ioopm_eq_function value_eq_func)
{
  elem_t *other_value_ptr = x;
  return value_eq_func(*other_value_ptr, value);
}

	  

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{
  ioopm_list_t *keys_list = ioopm_hash_table_keys(ht);
  ioopm_list_iterator_t *keys = ioopm_list_iterator(keys_list);
  ioopm_list_t *values_list = ioopm_hash_table_values(ht);
  ioopm_list_iterator_t *values = ioopm_list_iterator(values_list);

  for (int i = 0;ioopm_iterator_has_next(keys) ; ++i)
    {
      apply_fun(*ioopm_iterator_next(keys),
                ioopm_iterator_next(values),
                arg);
    }
  ioopm_iterator_destroy(keys);
  ioopm_linked_list_destroy(keys_list);

  ioopm_iterator_destroy(values);
  ioopm_linked_list_destroy(values_list);
}
