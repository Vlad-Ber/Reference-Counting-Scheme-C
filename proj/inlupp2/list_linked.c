#include "list_linked.h"
#include "iterator.h"
#include "common.h" 
#include <stdlib.h>
#include "refmem.h"


bool ioopm_int_eq(elem_t x, elem_t y) { return x.int_value == y.int_value; }
bool ioopm_unsigned_int_eq(elem_t x, elem_t y) { return x.us_value == y.us_value; }
bool ioopm_bool_eq(elem_t x, elem_t y) { return x.bool_value == y.bool_value; }
bool ioopm_float_eq(elem_t x, elem_t y) { return x.float_value == y.float_value; }


//typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

/*
TODO: -Linked_list_get segfaultar troligen med tom lista.
      -Vad returnar iterator_next om vi inte har en next?
*/
typedef struct iter ioopm_list_iterator_t;
typedef struct list ioopm_list_t;
typedef struct link ioopm_link_t;

struct link
{
  elem_t value;       // holds the key
  ioopm_link_t *next; // points to the next entry (possibly NULL)
};

struct shelf
{
  char *shelf_name;
  int quantity;
};

struct list
{
  ioopm_link_t *first;
  ioopm_link_t *last;
  size_t size;
  ioopm_eq_function eq_func;
};

struct iter 
{
  ioopm_link_t *current;
  ioopm_list_t *list;
};

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  ioopm_link_t *cursor = list->first;
  while (cursor != NULL)
    {
      ioopm_link_t *tmp = cursor;
      cursor = cursor->next;
      free(tmp);
      tmp = NULL;
    }
  free (list);
}
    
static ioopm_link_t *link_create(ioopm_link_t *next, elem_t value)
{
  ioopm_link_t *link = allocate(sizeof(ioopm_link_t),(void (*)(void *)) NULL );
  link->value = value;
  link->next = next;
  return link;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq)
{
  ioopm_list_t *list = allocate(sizeof(ioopm_list_t), (void (*)(void *)) ioopm_linked_list_destroy);
  list->last = NULL;
  ioopm_link_t *first = link_create(list->last, (elem_t){ .ptr_value = NULL });
  list->first = first;
  list->eq_func = eq;
  return list;
}

void ioopm_linked_list_shelf_destroy(ioopm_list_t *list)
{
  ioopm_link_t *cursor = list->first;
  while (cursor != NULL)
    {
      ioopm_link_t *tmp = cursor;
      cursor = cursor->next;
      if(cursor != NULL)
	{
	  free(cursor->value.shelf_value->shelf_name);
	  free(cursor->value.shelf_value);
	}
      free(tmp);
      tmp = NULL;
    }
  free (list);
}




static ioopm_link_t *previous_link(ioopm_list_t *list, size_t index)
{
  size_t listLength = ioopm_linked_list_size(list);

  if (index > listLength) 
    {
      index = listLength;
    }

  
  ioopm_link_t *cursor = list->first;
  while (index!=0) //CURSOR BLIR PEKARE TILL ELEMENTET INNAN INDEX (DUMMY)
    {
      cursor = cursor->next;
      index--;
    }
  return cursor;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
  ioopm_link_t *adress_to_last_element = list->last; 
  ioopm_link_t *new_link = link_create(NULL, value);
  if (adress_to_last_element != NULL)
    {
      adress_to_last_element->next = new_link;
      list->last = new_link;
    }
  else
    {
      list->first->next = new_link;
      list->last = new_link;    //BORDE BARA HÄNDA OM LISTAN ÄR TOM
    }
  list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
  ioopm_link_t *first_adress = list->first->next; //NEXT PGA DUMMY
  ioopm_link_t *new_link = link_create(first_adress, value);
  list->first->next = new_link;
  if (list->last==NULL)
    {
      list->last = new_link;
    }
  list->size++;
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
  ioopm_link_t *previous = previous_link(list, index);  
  ioopm_link_t *following_entry = previous->next;
  ioopm_link_t *new_link = link_create(following_entry, value);
  previous->next = new_link;
  
  if (previous == list->last) //UPPDATERA LAST OM VI STOPPAR IN PÅ SISTA PLATSEN
    {
      list->last = new_link;
    }
  list->size++;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
  //VAD HÄNDER OM VI FÖRSÖKER TA BORT I EN TOM LISTA?
  ioopm_link_t *previous = previous_link(list, index);
  ioopm_link_t *link_to_remove = previous->next;
  elem_t return_value = link_to_remove->value;
  previous->next = previous->next->next;
  free (link_to_remove);
  link_to_remove = NULL;
  
  //OM VI TAR BORT DET SISTA ELEMENTET BLIR ELEMENTET INNAN TILL LAST
  if (previous->next == NULL) 
    {
      list->last = previous;
    }
  list->size--;
  return return_value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
  ioopm_link_t *previous = previous_link(list, index);
  elem_t return_value = previous->next->value;
  return return_value;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
  ioopm_link_t *cursor = list->first->next;
  while (cursor != NULL)
    {
      if ( list->eq_func(cursor->value, element) )
	{
	  return true;
	}
      else
	{
	  cursor=cursor->next;
	}
    }
  return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  size_t size = list->size;
  return size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  ioopm_link_t *first = list->first->next;
  if (first == NULL)
    {
      return true;
    }
  else
    {
      return false;
    }
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  ioopm_link_t *cursor = list->first->next;
  while (cursor != NULL)
    {
      ioopm_link_t *tmp = cursor;
      cursor = cursor->next;
      free(tmp);
      tmp = NULL;
    }
  list->size = 0;
}


static bool linked_list_all_any_helper(bool b, ioopm_list_t *list, ioopm_predicate prop, void *extra)
{
  ioopm_link_t *cursor = list->first->next;
  elem_t value = cursor->value;
  elem_t key_ignored = {.int_value = 0};
  while (cursor != NULL)
    {
      value = cursor->value;
      if (b == prop(key_ignored, value, extra))
        {
          return b;
        }
      cursor = cursor->next;
    }
  return !b;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate prop, void *extra)
{
  return linked_list_all_any_helper(true, list, prop, extra);
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate prop, void *extra)
{
  return linked_list_all_any_helper(false, list, prop, extra);
}

void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_char_function fun, void *extra)
{
  ioopm_link_t *cursor = list->first->next;

  while (cursor != NULL)
    {
      elem_t *value = &(cursor->value);
      fun(0, value, extra);
      cursor = cursor->next;
    }
}

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *iter =  calloc(1, sizeof(ioopm_list_iterator_t));
  iter->list = list;
  iter->current = list->first;
  return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  bool result = true;
  if (iter->current->next == NULL)
    {
      result = false;
      return result;
    }
    
  return result;
   
}

elem_t *ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  if (ioopm_iterator_has_next(iter) == true)
    {
      iter->current = iter->current->next;
      return &iter->current->value;
    }
  return NULL;
  //VAD RETURNERAR VI OM VI INTE HAR EN NEXT????
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
  iter->current = iter->list->first;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
  elem_t current_value = iter->current->value;
  return current_value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  free(iter);
  iter = NULL;
}


