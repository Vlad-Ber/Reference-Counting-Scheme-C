#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "refmem.h"
#include <assert.h>

//branch test

struct objectInfo
{
  size_t rf;
  function1_t func;
};



obj *allocate(size_t bytes, function1_t destructor)
{
  void *data = calloc(1, sizeof(objectInfo_t) + bytes);
  objectInfo_t *objectToReturn = data + bytes - sizeof(objectInfo_t);

  objectToReturn->rf = 0;
  objectToReturn->func = destructor;

  return data;
}



void retain(obj *c)
{
  if(c == NULL){}
  else{
    objectInfo_t *objectInfo = c+sizeof(c);
   
    objectInfo->rf = objectInfo->rf+1;
    
    // printf("destructor = %p and rf is %p\n", objectInfo->func, objectInfo);
  }

}

void deallocate(obj *c){
  objectInfo_t *objectInfo = c+sizeof(c);
  function1_t destructor = objectInfo->func;
  printf("dealloc %p\n",(c+sizeof(c)));
  printf("adress of functions = %p and %p\n", objectInfo->func, destructor);
  destructor(c);
  free(c);
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
	  puts("ping");
	  deallocate(c);
	}
  
    }
}


size_t rc(obj *c)
{
  if(c== NULL)
    {
    return -1;
    }
  objectInfo_t *objectInfo = c+sizeof(c);
  return objectInfo->rf;
}




