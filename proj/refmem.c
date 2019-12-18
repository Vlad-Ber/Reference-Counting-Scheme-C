#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "refmem.h"
#include <assert.h>

size_t cascade_limit= 10;
obj *last_cascade = NULL;

obj *first_obj = NULL;
obj *last_obj = NULL;



struct objectInfo
{
  size_t rf;
  function1_t func;
  objectInfo_t *next;
};



obj *allocate(size_t bytes, function1_t destructor)
{
  obj *data = calloc(1, (sizeof(objectInfo_t) + bytes) );
  objectInfo_t *objectToReturn = data + bytes;

  printf("destructor alloc: %p \n", destructor);
  
  /*
  printf("sizeof data: %ld\n", sizeof(objectInfo_t) + bytes);
  
  printf("data p:%p\n", data);
  printf("data + obj p: %p\n",objectToReturn);
  printf("\n");
  */
  objectToReturn->rf = 0;
  objectToReturn->func = destructor;
  printf("destructor alloc2: %p \n", objectToReturn->func);

  
  if(first_obj == NULL){
    first_obj = data;
    last_obj = data;
  }
  else
    {
      objectInfo_t *last_obj_info = last_obj + sizeof(last_obj) - 1* sizeof(objectInfo_t) ;
      last_obj_info->next = data;
      last_obj = data;
    }
  
  
  return data;
}



void retain(obj *c)
{
  if(c == NULL){}
  else{
    objectInfo_t *objectInfo = c+sizeof(c) + sizeof(objectInfo_t);
   
    objectInfo->rf = objectInfo->rf+1;
  }

}

void deallocate(obj *c){
  objectInfo_t *objectInfo = c + sizeof(c) + sizeof(objectInfo_t) ;
  function1_t destructor = objectInfo->func;
  size_t temp = cascade_limit;
  printf("destructor dealloc:  %p \n", destructor);
  destructor(c);
  free(c);
  cascade_limit=temp+1;
  
}

void release(obj *c)
{
  if(c != NULL) 
    {
      objectInfo_t *objectInfo = c+sizeof(c);
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

void cleanup(){
  
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
  objectInfo_t *objectInfo = c+sizeof(c) + sizeof(objectInfo_t);
  return objectInfo->rf;
}




