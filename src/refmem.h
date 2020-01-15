#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


/**
 * @file refmem.h
 * @authors Adam Axelsson, Vlad Bertilsson,  Carl Hasselberg, Ellinor Hallmén,
Ali Hoseini, Erik Junghahn, Miranda Kiss-Deáki
 * @date 200114
 * @brief Functions for allocating and deallocating data, by keeping
 * track of pointers and manually managing memory with pointer arithmatics.
 *
 * @see http://wrigstad.com/ioopm19/projects/project1.html
 */

/// @biref object, for instance a hashtable or a cell
typedef void obj;

/// @brief destroy functions that applied to objects
typedef void(*function1_t)(obj *);

/// @brief information about object: number of pointers, size of object and destroy function
typedef struct objectInfo objectInfo_t;


/// @brief increments reference counter (rf) of an object
/// @param obj* an object
void retain(obj *);

/// @brief decrements rf of an object
/// @param obj* an object
void release(obj *);

/// @brief returns rf of an object from its objectInfo
/// @param obj* an object
/// @return the amount of references that object has in the form of a size_t, -1 if object is null
size_t rc(obj *);

/// @brief allocates memory for an object and its objectInfo, which contains rf 0, a destructor function and the size
/// @param bytes the size of the object in size_t
/// @param destructor the object's destroy function
/// @return an obj* pointer to the object
obj *allocate(size_t bytes, function1_t destructor);

/// @brief allocates an array for all objectInfos
/// @param elements amount of objectInfos
/// @param elem_size size of one element
/// @param destructor destroy funtion
/// @return an obj* pointer to array
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);

/// @brief frees the memory used by an object and its objectInfo
/// @param obj* an object
void deallocate(obj *);

/// @brief sets global variable cascade limit to a new value
/// @param limit size_t of chosen limit
void set_cascade_limit(size_t limit);

/// @brief gets the value of current cascade limit
/// @return size_t cascade limit
size_t get_cascade_limit();

/// @brief sets all rfs to 0 and
void cleanup();


//void remove_next_link(objectInfo_t *info);
//void remove_this_link(objectInfo_t *info);



/// @brief deallocates everything with one call
void shutdown();

