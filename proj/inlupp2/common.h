#ifndef _COMMONH_
#define _COMMONH_

#include <stdbool.h>
#include <string.h>


typedef struct list ioopm_list_t;
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef struct link link_t;
typedef struct iter ioopm_list_iterator_t;
typedef struct cart ioopm_cart_t;
typedef struct merch ioopm_merch_t;
typedef struct shelf ioopm_shelf_t;
typedef struct database ioopm_db_t;
typedef struct order ioopm_order_t;

typedef union elem elem_t;

union elem
{
  int int_value;
  unsigned int us_value;
  bool bool_value;
  float float_value;
  char * str_value;
  void *ptr_value;
  ioopm_merch_t *merch_value;
  ioopm_shelf_t *shelf_value;
  ioopm_order_t *order_value;
  ioopm_cart_t *cart_value;
};

/// Compares two elements and returns true if they are equal
typedef int (*ioopm_eq_function)(elem_t a, elem_t b);

typedef bool (*ioopm_char_predicate)(void *, void *);
typedef void (*ioopm_apply_char_function)(int ,elem_t *, void *);
typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
typedef bool (*ioopm_predicate_extended)(elem_t key, elem_t value, void *extra, ioopm_eq_function compare); // andreas
typedef void (*ioopm_function)(elem_t key, elem_t *value, void *extra);
typedef int (*ioopm_hash_function)(elem_t key);

#endif


