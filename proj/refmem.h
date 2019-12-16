//#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
//#include "hash_table.h"
//#include "linked_list.h"


typedef struct object obj;
typedef void(*function1_t)(obj *);

struct object
{
  int rf;
  function1_t *func;
  void *object;
};


obj *allocate(size_t bytes, function1_t *destructor){
  obj *objectToReturn = calloc(1, sizeof(obj));
  void *data = calloc(1, bytes);
  objectToReturn->rf = 0;
  objectToReturn->func = destructor;
  objectToReturn->object = data;
  return data;
}

/*
void retain(obj *c)
{
  
  c.rf++;
}
    
void release (obj *c)
{
  c.rf--;
}

*/
void retain(obj *);
void release(obj *);
size_t rc(obj *);
obj *allocate(size_t bytes, function1_t *destructor);
obj *allocate_array(size_t elements, size_t elem_size, function1_t *destructor);
void deallocate(obj *);
void set_cascade_limit(size_t);
size_t get_cascade_limit();
void cleanup();
void shutdown();

int main()
{
  return 0;
}

