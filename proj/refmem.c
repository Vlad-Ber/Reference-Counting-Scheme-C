#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "refmem.h"
#include <assert.h>

size_t cascade_limit= 10;
obj *last_cascade = NULL;

objectInfo_t *first_info = NULL;
objectInfo_t *last_info = NULL;



struct objectInfo
{
  size_t rf;
  function1_t func;
  objectInfo_t *next;
};



obj *allocate(size_t bytes, function1_t destructor)
{
  obj *data = calloc(1, (sizeof(objectInfo_t) + bytes) );

  objectInfo_t *objectToReturn = data;
  data = data + sizeof(objectInfo_t);
  
  printf("destructor alloc: %p \n", destructor);
  
  objectToReturn->rf = 0;
  objectToReturn->func = destructor;
  printf("destructor alloc2: %p \n", objectToReturn->func);

  
  if(first_info == NULL)
    {
      first_info = objectToReturn;
      last_info = objectToReturn;
    }
  else
    {
      last_info->next = objectToReturn ;
      last_info = objectToReturn;
    }
  
  
  return data;
}



void retain(obj *c)
{
  if(c == NULL){}
  else{
    objectInfo_t *objectInfo = c - sizeof(objectInfo_t);
    objectInfo->rf = objectInfo->rf+1;
  }

}

void deallocate(obj *c){
  objectInfo_t *objectInfo = c  -  sizeof(objectInfo_t);
  function1_t destructor = objectInfo->func;  
  size_t temp = cascade_limit;
  printf("destructor dealloc:  %p \n", destructor);
  destructor(c);
  free(objectInfo);
  cascade_limit=temp+1;
  
}

void release(obj *c)
{
  if(c != NULL) 
    {
      objectInfo_t *objectInfo = c  - sizeof(objectInfo_t);
      if(objectInfo->rf != 0)
        {
          objectInfo->rf--;
        }
      if(objectInfo->rf == 0)
        {
          cascade_limit = cascade_limit - 1;

          if(cascade_limit > 0)
            {
              deallocate(c);
            }
          else 
            {
              last_cascade = c;
            }
        }
    }
}

void remove_next_link(objectInfo_t *trav){
  objectInfo_t *to_remove_inf = trav->next;
  trav->next = trav->next->next;

  printf("destructor in remove:  %p\n", to_remove_inf->func);

  
  obj *to_remove = to_remove_inf + sizeof(objectInfo_t);

  printf("to_remove: %p \n", to_remove);
  
  release(to_remove);
}

void cleanup(){

  objectInfo_t *trav = first_info;
  
  if(trav == NULL)
    {
      return;
    }
  
  while(trav->next != NULL)
    {
      printf("rf in cleanup:  %ld\n",trav->next->rf);
      printf("destructor in cleanup:  %p\n", trav->next->func);
    if(trav->next->rf == 0){
      remove_next_link(trav);
      
    }
    trav = trav->next;
  }
  
  if(first_info->rf == 0){
    objectInfo_t *new_first = first_info->next;
    deallocate(first_info + sizeof(objectInfo_t));
    first_info = new_first;
  }
}


void set_cascade_limit(size_t size)
{
  cascade_limit = size;
}

size_t get_cascade_limit()
{
  return cascade_limit;
}

  


size_t rc(obj *c)
{
  if(c== NULL)
    {
    return -1;
    }
  objectInfo_t *objectInfo = c+sizeof(*c) - sizeof(objectInfo_t);
  return objectInfo->rf;
}




