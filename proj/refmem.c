#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "refmem.h"


struct objectInfo
{
  int rf;
  function1_t *func;
  //
};





obj *allocate(size_t bytes, function1_t *destructor){
  
void *data = malloc(sizeof(objectInfo_t) + bytes);
objectInfo_t *objectToReturn = data + bytes;
objectToReturn->rf = 0;
objectToReturn->func = destructor;
//objectToReturn->object = data;
return data;
}

/*
void retain(obj *c)
{
  

}
/*   
void release (obj *c)
{
  c.rf--;
  }*/

int main()
{
  
return 0;
}

