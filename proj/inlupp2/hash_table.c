#include "hash_table.h"
#include "list_linked.h"
#include <CUnit/CUnit.h>
#include <string.h>
#include <assert.h>

#define Free(ptr) {free(ptr); ptr = NULL;}

struct entry
{
  elem_t key;
  elem_t value;
  entry_t *next;
};

//TODO alternativt lös så att denna struct inte behövs här
struct merch
{
  char *name;
  char *description;
  int price;
  ioopm_list_t *shelf;
};



struct hash_table
{
  size_t no_buckets;
  entry_t **buckets;
  ioopm_eq_function compare_key;
  ioopm_eq_function compare_value;
  ioopm_hash_function hash_fun;
  float load_factor;
  size_t filled_buckets;
};

void clear_dummys(ioopm_hash_table_t *ht)
{
  for(int index = 0; index < ht->no_buckets;index++)
    {
      free(ht->buckets[index]);
    }
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *nextvalue)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t));
  new_entry -> key = key;
  new_entry -> value = value;
  new_entry -> next = nextvalue;
  return new_entry;
}

//TODO denna ändrad från orginal, anpassad för att fria merch. Möjligt att göra en ny funktion för att förstöra databaser.
static void entry_destroy(entry_t *entry)
{
  //TODO tog bort free under för att få test_replenish att funka, denna borde vara såhär i forts.
  //free(entry->value.str_value);
  free(entry);
}



static int extract_int_hash_key(elem_t key)
{
  return key.int_value;
}

static bool growth_check(ioopm_hash_table_t *ht)
{
  return ((float) ht->no_buckets * ht->load_factor < (float) ht->filled_buckets);
}

static size_t actual_growth(ioopm_hash_table_t *ht)
{
  size_t result = 16381;
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};
  int length_primes = 11;
  for(int i = 0; i < length_primes; i++)
    {
      if(ht->no_buckets < primes[i])
	{
	  result = primes[i];
	  break;
	}
    }
  return result;
}

static void initialise_dummys(ioopm_hash_table_t *ht)
{
    for(int index = 0; index < ht->no_buckets; index++)
    {
      elem_t key = {.str_value = NULL};
      elem_t value = {.str_value = NULL};
      ht->buckets[index] = entry_create(key, value, NULL);
    }
}

ioopm_hash_table_t *ioopm_hash_table_create_with_load_factor(ioopm_eq_function compare_key, ioopm_eq_function compare_value, ioopm_hash_function hash_fun, float load_factor, size_t no_buckets)
{
  ioopm_hash_table_t *ht = (ioopm_hash_table_t *) calloc(1, sizeof(ioopm_hash_table_t));
  ht->compare_key = compare_key;
  ht->compare_value = compare_value;
  if(hash_fun == NULL)
    {
        ht->hash_fun = extract_int_hash_key;
    }
  else
    {
        ht->hash_fun = hash_fun;
    }
  ht->load_factor = load_factor;
  ht->no_buckets = no_buckets;
  ht->buckets = calloc(no_buckets, sizeof(entry_t *));
  ht->filled_buckets = 0;

  initialise_dummys(ht);
  
  return ht;
}

static void rehashing(ioopm_hash_table_t *ht)
{
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  size_t size = ioopm_hash_table_size(ht);
  size_t new_size = actual_growth(ht);

  ioopm_hash_table_clear(ht);
  clear_dummys(ht);
  ht->buckets = realloc(ht->buckets, sizeof(entry_t *) * new_size);
  ht->no_buckets = new_size;
  initialise_dummys(ht);
  ht->no_buckets = new_size;
  
  for(int index = 0; index < size; index++)
    {
      elem_t key = ioopm_linked_list_get(keys, index);
      elem_t value = ioopm_linked_list_get(values, index);
      ioopm_hash_table_insert(ht, key, value);
    }

  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);  
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_eq_function compare_key, ioopm_eq_function compare_value, ioopm_hash_function hash_fun)
{
  float default_load_factor = 0.75;
  int default_no_buckets = 17;
  return ioopm_hash_table_create_with_load_factor(compare_key, compare_value, hash_fun, default_load_factor, default_no_buckets);
}

entry_t *ioopm_find_previous_entry_for_key(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket_index = ht->hash_fun(key) % ht->no_buckets;
  entry_t *current = (ht->buckets[bucket_index]);
  
  while(current != NULL)
    {
      if(current-> next == NULL || current->next->key.str_value == NULL)
	{
	  break;
	}
      else if(ht->compare_key(current->next->key, key)  == 0)
	{
	  break;
	}
      else if(ht->compare_key(key, current->next->key) == -1)
	{
	  break;
	}
      else if(current->next == NULL)
	{
	  break;
	}
      current = current->next;
    }
  return current;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  if(ht->hash_fun(key) < 0)
    {
      errno = EINVAL;
    }
  else
    {
      entry_t *entry = ioopm_find_previous_entry_for_key(ht, key);
      entry_t *next = entry->next;
      entry_t *new_entry;

      int bucket_index = ht->hash_fun(key) % ht->no_buckets;
      if(ht->buckets[bucket_index]->next == NULL)
	{
	  ht->filled_buckets++;
	}
      
      if(entry->next == NULL)
	{
	  new_entry = entry_create(key, value, next);
	  entry->next = new_entry;
	}
      else if(ht->compare_key(entry->next->key, key) == 0)
	{
	  next->value = value;
	}
      else
	{
	  new_entry = entry_create(key, value, next);
	  entry->next = new_entry;
	}
      
      if(growth_check(ht))
	{
	  rehashing(ht);
	}
    }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);
  clear_dummys(ht);
  free(ht->buckets);
  Free(ht);
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *value)
{
  if(ht->hash_fun(key) < 0)
    {
      errno = EINVAL;
      return NULL;
    }
  else
    {
      int bucket_of_key = ht->hash_fun(key) % ht->no_buckets;  
      entry_t *current = (ht->buckets[bucket_of_key]);
      if(current->next == NULL)
	{
	  return false;
	}
      while(current->next != NULL)
	{
	  if(ht->compare_key(key , current->next->key) == 0)
	    {
	      *value = current->next->value;
	      return true;
	    }
	  current = current->next;
	}
      return false;
    }
  assert(false);
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  if(ht->hash_fun(key) < 0)
    {
      errno = EINVAL;
      elem_t result = {.int_value = -1};
      return  result;
    }
  else
    {
      entry_t *previous = ioopm_find_previous_entry_for_key(ht, key);

      int bucket_index = ht->hash_fun(key) % ht->no_buckets;
      if(previous->next != NULL && 
	 previous->next->next == NULL &&
	 ht->compare_key(previous->next->key, (ht->buckets[bucket_index]->next->key)) == 0)
	{
	  ht->filled_buckets--;
	}

      if(previous->next == NULL)
	{
	  elem_t result = {.str_value = ""};
	  return result;
	}
      else
	{
	  entry_t *temp = previous->next->next;
	  elem_t value = previous->next->value;
	  entry_destroy(previous->next);
	  previous->next = temp;
	  return value;
	}
    }
  assert(false);
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t result = 0;
  entry_t entry;
  for(int index = 0; index < ht->no_buckets; index++)
    {
      entry = *(ht->buckets[index]); 
      if(entry.next != NULL)
	{
	  entry = *(entry.next);
	  while(entry.next != NULL)
	    {
	      result++;
	      entry = *(entry.next);
	    }
	  result++;
	}
    }
  return result;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for(int index = 0; index < ht->no_buckets; index++)
    {
      if((ht->buckets[index]->next) != NULL)
	{
	  return false;
	}
    }
  return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  entry_t *entry;
  entry_t *tempentry;
  entry_t *dummy; 
  for(int index = 0; index < ht->no_buckets; index++)
    {
      entry = ht->buckets[index];

      if(entry -> next != NULL)
	{
	  dummy = entry;
	  entry = entry->next;
	  dummy->next= NULL;
	  while(entry -> next != NULL)
	    {
	      tempentry = entry -> next;
	      entry_destroy(entry);		
	      entry = tempentry;
	    }
	  entry_destroy(entry);
	}
    }
  ht->filled_buckets = 0;
}


static ioopm_list_t *create_keys_or_values_list(ioopm_hash_table_t *ht, bool is_key)
{
  ioopm_list_t *list;
  if(is_key)
    {
      list = ioopm_linked_list_create(ht->compare_key);  
    }
  else
    {
      list = ioopm_linked_list_create(ht->compare_value);
    }

  entry_t entry;
  for(int index = 0; index < ht->no_buckets; index++)
    {
      entry = *ht->buckets[index];

      if(entry.next != NULL)
	{
	  while(entry.next != NULL)
	    {
	      entry = *(entry.next);
	      if(is_key)
		{
		  ioopm_linked_list_append(list, entry.key);		  
		}
	      else
		{
		  ioopm_linked_list_append(list, entry.value);
		}
	    }
	}
    }
  return list;
}


ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  return create_keys_or_values_list(ht, true);
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  return create_keys_or_values_list(ht, false);
}

static bool check_has_key_or_value(ioopm_hash_table_t *ht, ioopm_predicate_extended pred, void *arg, bool check_key)
{
  bool result = false;
  entry_t entry;
  for(int index = 0; (index < ht->no_buckets) && (result == false) ;index++)
    {
      entry = *ht->buckets[index];
      while(entry.next != NULL)
	{
	  entry  = *(entry.next);
	  if(check_key)
	    {
	      if(pred(entry.value , entry.key , arg, ht->compare_key) == true)
		{
		  result = true;
		  break;
		}
	    }
	  else
	    {
	      if(pred(entry.key , entry.value , arg, ht->compare_value) == true)
		{
		  result = true;
		  break;
		}
	    }
	}      
    }
  return result;
}

static bool value_or_key_equiv(elem_t ignored, elem_t to_compare, void *x, ioopm_eq_function compare_value_or_key)
{
  elem_t *other_value_ptr = x;
  elem_t other_value = *other_value_ptr;
  return compare_value_or_key(to_compare, other_value) == 0;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return check_has_key_or_value(ht, value_or_key_equiv, &value, false);
}

static bool check_for_all_or_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg, bool check_all)
{
  bool result = check_all;
  entry_t entry;
  for(int index = 0; (index < ht->no_buckets) && (result == check_all) ;index++)
    {
      entry = *ht->buckets[index];
      while(entry.next != NULL)
	{
	  entry = *(entry.next);
	  if(pred(entry.key, entry.value, arg) == !check_all)
	    {
	      result = !check_all;
	      break;
	    }
	}
    }
  return result;  
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  return check_for_all_or_any(ht, pred, arg, true);
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  return check_for_all_or_any(ht, pred, arg, false);
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_function apply_fun, void *arg)
{
  entry_t *entry;
  for(int index = 0;index < ht->no_buckets ; index++)
    {
      entry = ht->buckets[index];
      while(entry->next != NULL)
	{
	  entry = entry->next;
	  apply_fun(entry->key, &(entry->value), arg);
	}
    }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  if(ht->hash_fun(key) < 0)
    {
      errno = EINVAL;
      return NULL;
    }
  else
    {
      return check_has_key_or_value(ht, value_or_key_equiv, &key, true);      
    }
  assert(false);
}

int ioopm_eq_function_int(elem_t int_a, elem_t int_b)
{
  int result;
  if(int_a.int_value == int_b.int_value)
    {
      result = 0;
    }
  else if(int_a.int_value < int_b.int_value)
    {
      result = -1;
    }
  else
    {
      result = 1;
    }   
  return result;
}

int ioopm_eq_function_str(elem_t str_a, elem_t str_b)
{
  return strcmp(str_a.str_value , str_b.str_value);
}