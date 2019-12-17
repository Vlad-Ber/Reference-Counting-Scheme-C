#pragma once
#include <stdbool.h>
#include "list_linked.h"

// hej
#define ioopm_int_iterator_next(iter)           \
    ioopm_iterator_next(iter)->i
#define ioopm_int_iterator_current(iter)           \
    ioopm_iterator_current(iter).i


typedef struct iter ioopm_list_iterator_t;

/// @brief Creates an iterator for a list
/// @param list the list
/// @return an iterator for list
ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list);
/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if
bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
/// @return the next element
/// @pre Undefined behaviour when there is no next element.
elem_t *ioopm_iterator_next(ioopm_list_iterator_t *iter);
/*
/// NOTE: REMOVE IS OPTIONAL TO IMPLEMENT 
/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
int ioopm_iterator_remove(ioopm_list_iterator_t *iter);

/// NOTE: INSERT IS OPTIONAL TO IMPLEMENT 
/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_iterator_insert(ioopm_list_iterator_t *iter, int element);
*/
/// @brief Reposition the iterator at the start of the underlying list
/// @param iter the iterator
void ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @return the current element
///@pre Undefined behaviour if ioopm_iterator_next has been called but no next element exists
elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_iterator_destroy(ioopm_list_iterator_t *iter);
