#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "refmem.h"
#include <assert.h>
size_t cascade_limit= 1;
obj *temp_obj= NULL;


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
  }

}

void deallocate(obj *c){
  objectInfo_t *objectInfo = c+sizeof(c);
  function1_t destructor = objectInfo->func;
  size_t temp = cascade_limit;
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

	  if(cascade_limit != 0)
	    {
	      printf("casdadelimit is %ld\n", cascade_limit);
	      deallocate(c);
	    }
	
  
	}
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
  objectInfo_t *objectInfo = c+sizeof(c);
  return objectInfo->rf;
}




