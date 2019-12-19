#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "refmem.h"
#include <assert.h>

size_t cascade_limit= 10;
obj *last_cascade = NULL;

objectInfo_t *first_info = NULL;
objectInfo_t *last_info = NULL;
//


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
   remove_this_link(objectInfo);
  destructor(c);
  //fixa så att när vi delocatar att länken inte finns kvar
  //vi måst komma åt den som e bakom right? wtf huuur?

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

//////////////////////////////////
objectInfo_t *find_previous_link(objectInfo_t *this_link){
  printf("find precvious link check \n");
  // detta är ganska kefft men kommer nog funka iaf. dock långsamt.
  objectInfo_t *search = first_info;
  while(search->next != this_link && search->next != NULL){
    printf("search->next : %p\n", search->next);
    search = search->next;
  }
  printf("while done on find prev \n");
  if(search->next == NULL){
    return this_link; // vettefan vad som ska hända då tbh. den här funktionen ska ändå vara gömd för användaren.
  }
  else{
    return search;
  }
  
}
// brehs this is radical
void remove_this_link(objectInfo_t *info){

  if(info == first_info){
    first_info = info->next;
    return;
  }
  
  printf("remove this link check: %p\n", info);
  // vi måste hitta länken bakom oss right? men det gårt ju inte.
  // Scheiße. vi får srkiva en find previous link funcktion :(((
  objectInfo_t *prev = find_previous_link(info);

  prev->next = info->next;
}
///////////////////////////////////



void remove_next_link(objectInfo_t *trav){
  objectInfo_t *to_remove_inf = trav->next;
  trav->next = trav->next->next;
  
  printf("trav next: %p \n", trav->next);

   long long to_remove_inf_adr = (long long) to_remove_inf;  // xd
   obj *to_remove = ((void *) to_remove_inf_adr + sizeof(objectInfo_t));

   to_remove_inf->func(to_remove);
   free(to_remove_inf);
   
   //deallocate(to_remove);
}

void cleanup(){

  objectInfo_t *trav = first_info;
  
  if(trav == NULL)
    {
      return;
    }
  
  while(trav->next != NULL) 
    {
      printf("---helklo \n");
      printf("rf in cleanup:  %ld\n",trav->next->rf);
      printf("destructor in cleanup:  %p\n", trav->next->func);

      if(trav->next->rf == 0){
      remove_next_link(trav);
      
    }
      printf("trav almost at end of while: %p\n", trav);
      if(trav->next != NULL)
        {
      trav = trav->next;
        }
      printf("alive \n");
      printf("trav at end of while: %p\n", trav);

      printf("rf trav at end: %ld \n", trav->rf);
  }


  
  if(first_info->rf == 0){
    
    objectInfo_t *new_first = first_info->next;
     printf("first_info func:%p\n", first_info->func);

     long long first_info_adr = (long long) first_info; // detta e så facking sjukt

     obj *first_obj = ( (void *) first_info_adr + sizeof(objectInfo_t));

     //mer eller mindre en deallocate, fast just nu struntar vi i cascade_limit
     first_info->func(first_obj);
     free(first_info);
    
     
    
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
  objectInfo_t *objectInfo = c - sizeof(objectInfo_t);
  return objectInfo->rf;
}




