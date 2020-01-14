#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


/**
 * @file refmem.h
 * @author 
 * @date
 * @brief Simple hash table that maps integer keys to string values.
 *
 * 
 * 
 * 
 *
 * @see http://wrigstad.com/ioopm19/projects/project1.html
 */


typedef void obj;
typedef void(*function1_t)(obj *);
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

/// @brief 
/// @param
/// @param
/// @return
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

/// @brief 
/// @param
/// @param
/// @return
void cleanup();

/// @brief 
/// @param
/// @param
/// @return
void shutdown();

/// @brief 
/// @param
/// @param
/// @return
void remove_next_link(objectInfo_t *info);

/// @brief 
/// @param
/// @param
/// @return
void remove_this_link(objectInfo_t *info);
