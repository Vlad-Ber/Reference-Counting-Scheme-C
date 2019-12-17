#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "refmem.h"

//branch test

struct objectInfo
{
  size_t rf;
  function1_t func;
};

struct cell
{
  struct cell *cell;
  int i;
  char *string;
};

void cell_destructor(obj *c)
{
  
  release(((struct cell *) c)->cell);
  free(c);
}


obj *allocate(size_t bytes, function1_t destructor){
  
void *data = malloc(sizeof(objectInfo_t) + bytes);
objectInfo_t *objectToReturn = data + bytes;
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
  //destructor(c);
  cell_destructor(c);
}

void release(obj *c)
{
  if(c== NULL){}
  else
    {
      objectInfo_t *objectInfo = c+sizeof(c);
      if(objectInfo->rf == 0)
	{
	  deallocate(c);
	}
      else
	{
	  objectInfo->rf = objectInfo->rf-1;
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


int main()
{
  struct cell *c = allocate(sizeof(struct cell), cell_destructor);
  //int *c = NULL; //får segfault
  
  //*c = 5;
    
  retain(c);
  //printf("rc av c = %ld\n", rc(c));
  release(c);
  //printf("rc av c = %ld\n", rc(c));
  release(c);
  //printf("rc av c = %ld\n", rc(c));
    
  return 0;
}

