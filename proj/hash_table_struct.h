#ifndef __HASH_TABLE_STRUCT_H_
#define __HASH_TABLE_STRUCT_H_
#include "list_linked.h"

/// Only used for internal tests

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};



struct hash_table
{
  entry_t **buckets;
  int size;
  cmp_fun_t compare_func;
  hash_fun_t hash_function;
  ioopm_eq_function key_eq_function;
  ioopm_eq_function value_eq_function;
  size_t nr_buckets_index;
  double load_factor;
};

#endif // __HASH_TABLE_STRUCT_H_
