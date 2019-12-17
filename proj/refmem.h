#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "hash_table.h"
//#include "refmem.c"



typedef void obj;
typedef void(*function1_t)(obj *);
typedef struct objectInfo objectInfo_t;



void retain(obj *);
void release(obj *); //Ska returnera void
size_t rc(obj *);
obj *allocate(size_t bytes, function1_t destructor);
obj *allocate_array(size_t elements, size_t elem_size, function1_t *destructor);
void deallocate(obj *);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();
