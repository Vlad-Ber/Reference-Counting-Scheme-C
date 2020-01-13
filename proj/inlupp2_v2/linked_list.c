#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "linked_list.h"
#include "refmem.h"

struct list
{
  size_t size;
  link_t *head;      // points to first entry
  link_t *tail;      // points to last entry
  ioopm_eq_function equals;
};

struct link
{
  elem_t val;
  link_t *next;     //points to next link
};

static void doNothing(obj *c){}
int ioopm_list_size(ioopm_list_t *list)
{
  return(list->size);
}


ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function equal) 
{
  ioopm_list_t *list = allocate(sizeof(ioopm_list_t), doNothing);
  retain(list);
  link_t *dummy = allocate(sizeof(link_t), doNothing);
  retain(dummy);
  list->equals = equal;
  list->head = dummy;
  list->tail = dummy;
  list->size = 0;
  return list;
}

static link_t *link_create(elem_t val, link_t *next) 
{
  link_t *link = allocate(sizeof(link_t), doNothing);
  retain(link);
  link->val = val;
  link->next = next;
  return link;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t val)
{
  link_t *tmp = list->tail->next;
  link_t *new_entry = link_create(val, tmp);
  list->tail->next = new_entry;
  list->tail = new_entry;
  list->size++;
}


void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t val)
{
  link_t *tmp = list->head->next;
  link_t *new_entry = link_create(val, tmp);
  list->head->next = new_entry;
  if (list->head == list->tail)
    {
      list->tail = new_entry;
    }
  list->size++;
}

static void link_destroy(link_t *link)
{
  release(link);
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  ioopm_linked_list_clear(list);
  release(list->head);
  release(list);
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  link_t *link = list->head;
  link_t *next_link = link->next;
  link_t *tmp;
  while(next_link != NULL)
    {
      tmp = next_link->next;
      link->next = next_link->next;
      link_destroy(next_link);
      next_link = tmp;
    }
  list->size = 0;
}


static link_t *find_previous_link(ioopm_list_t *list, size_t index)
{
  link_t *previous = list->head;
  link_t *next_entry = previous->next;
  for (size_t i = 0; i < index; i++)
    {
      previous = next_entry;
      next_entry = next_entry->next;
    }
  return previous;
}

option_t ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t val)
{
  if (index < 0 || index > list->size)
    {
      return Failure();
    }
  else
    {
      link_t *previous = find_previous_link(list, index);
      link_t *next_link = previous->next;
      
      previous->next = link_create(val, next_link);
      list->size++;
      if (previous->next->next == NULL)
        {
          list->tail = previous->next;
        }
      return Success(val);
    }
}


option_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
 
  if (list->size == 0 || index < 0 || index > list->size - 1)
    {
      return Failure();
    }
  else
    {
      link_t *previous_link = find_previous_link(list, index);
      link_t *link_to_be_removed = previous_link->next;
      previous_link->next = link_to_be_removed->next;
      elem_t val = link_to_be_removed->val;
      link_destroy(link_to_be_removed);
      list->size--;
      return Success(val);
    }
}

option_t ioopm_linked_list_get(ioopm_list_t *list, size_t index)
{
  if (list->size == 0 || index < 0 || index > list->size - 1)
    {
      return Failure();
    }
  else
    {
      link_t *previous_link = find_previous_link(list, index);
      link_t *link = previous_link->next;
      return Success(link->val);
    }
}


bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, ioopm_eq_function equals)
{
  link_t *dummy_link = list->head;
  link_t *current_link = dummy_link->next;
  while(current_link)
    {
      if (equals(current_link->val, element))  
        {
          return true;
        }
      current_link = current_link->next;
    }
  return false;
}

 
size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->size;
}
 
bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  return !list->size;
}
 
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate prop, void *extra){
  if (!ioopm_linked_list_is_empty(list))
    {
      link_t *current = list->head->next;
      while (current)
        {
          if (!prop(current->val, current->val, extra))
            {
              return false;
            }
          current = current->next;
        }
    }
  return true;
}
 
 
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate prop, void *extra){
  if (!ioopm_linked_list_is_empty(list))
    {
      link_t *current = list->head->next;
      while (current)
        {
          if (prop(current->val, current->val, extra))
            {
              return true;
            }
          current = current->next;
        }
    }
  return false;
}
 
 
void val_add(int *val, void *extra)
{
  int *val2 = extra;
  *val += *val2;
}
 
 
void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, void *extra)
{
  if (!ioopm_linked_list_is_empty(list))
    {
      link_t *current = list->head->next;
      while (current)
        {
          fun(current->val, &current->val, extra);
          current=current->next;
        }
      
    }
}


