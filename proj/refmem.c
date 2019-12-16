#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "hash_table.h"
#include "refmem.h"
#include "common.h"


static bool intequalslist(elem_t a, elem_t b){
  int inta = a.i;
  int intb = b.i;
  return inta == intb;
}

ioopm_hash_table_t *hashTable = ioopm_hash_table_create(intequalslist,NULL,intequalslist, intequalslist);

struct objectInfo
{
  int rf;
  function1_t *func;
  //obj *object;
};





obj *allocate(size_t bytes, function1_t *destructor){
  
  struct objectInfo *objectToReturn = calloc(1, sizeof(obj));
  void *data = calloc(1, bytes);
  objectToReturn->rf = 0;
  objectToReturn->func = destructor;
  //objectToReturn->object = data;
  return data;
}

/*
void retain(obj *c)
{
  
  ((struct objectInfo *)c)->rf++;
}
    
void release (obj *c)
{
  c.rf--;
  }*/

int main()
{
  printf("%d\n", ht->size);
  
  
  return 0;
}

