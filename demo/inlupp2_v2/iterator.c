#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "linked_list.h"
#include "iterator.h"

typedef struct iter ioopm_list_iterator_t;

struct iter
{
    link_t *current;
    ioopm_list_t *list; /// New field
    ioopm_eq_function equals;
};

struct link
{
    elem_t val;
    link_t *next;     //points to next link
};

struct list
{
    size_t size;
    link_t *head;       // points to first entry
    link_t *tail;    // points to last entry
};


ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *list)   //creates iterator
{
    ioopm_list_iterator_t *iter = calloc(1, sizeof(struct iter));
    iter->current = list->head->next;  //hoppar över dummy
    iter->list = list;
    return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)   //checks if next entry is null
{
    if (iter->current == NULL)
    {
        return false;
    }

    if (iter->current->next == NULL)
    {
        return false;
    }
    else return true;
}

ioopm_option_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    if (ioopm_iterator_has_next(iter) == false)
    {
        return Failure();
    }
    else
    {
        iter->current = iter->current->next;
    }
    return Success(iter->current->val);
}

ioopm_option_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    if (iter->current == NULL)
    {
        return Failure();
    }
    return Success(iter->current->val);
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter) //resets iter to head +1
{
    iter->current = iter->list->head->next;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

