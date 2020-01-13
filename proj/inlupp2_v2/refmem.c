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
  
  objectInfo_t *next;
  size_t size;
  function1_t func;
};

void insert(objectInfo_t *objectToInsert)
{
    if(first_info == NULL)
    {
      first_info = objectToInsert;
      last_info = objectToInsert;
    }
  else
    {
      last_info->next = objectToInsert;
      last_info = objectToInsert;
    }
}

void default_destructor(obj *c){
  //generic comment
  printf("\n \n-----DEFAULT DESTRUCTOR \n \n");
  objectInfo_t *current_info = first_info;
  objectInfo_t *c_info = c - sizeof(objectInfo_t);
  
  for(size_t i=0; i< c_info->size ; i++){

    void **possible_pointer = c + i;
    
    current_info = first_info;
    //  printf("i: %ld\n", i);
    while(current_info != NULL){
     
      //  printf("current info: %p ,", current_info);
      //printf("c+i: %p: \n", c+i);
       obj *current_obj = (void *) (long long) current_info + sizeof(objectInfo_t);

       //printf("current_obj and pp: (%p, %p) \n", current_obj, possible_pointer);
       //  printf("diff: %ld", current_obj - possible_pointer );
       
      //printf(" c+i and current_obj diff: %ld \n",  (c+i) - current_obj );
       if( possible_pointer == current_obj ) {
         // printf("if == true \n");
         //printf("c+i och current_obj: (%p , %p) \n", possible_pointer, current_obj);
        release(*possible_pointer);
        break;
      }
      current_info = current_info->next;
    }
    
  }
}

obj *allocate(size_t bytes, function1_t destructor)
{
  obj *data = calloc(1, (sizeof(objectInfo_t) + bytes) );

  objectInfo_t *objectToReturn = data;
  data = data + sizeof(objectInfo_t);
  

  objectToReturn->rf = 0;
  objectToReturn->func = destructor;
  objectToReturn->size = bytes;


  
  insert(objectToReturn);
  
  return data;
}

obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor)
{
  void *data = calloc(elements, ((sizeof(objectInfo_t)/elements) +1 + elem_size));

  objectInfo_t *objectToReturn = data;
  objectToReturn->func = destructor;
  objectToReturn->size = elem_size*elements;
  objectToReturn->rf=0;
  insert(objectToReturn);


  
  return data+sizeof(objectInfo_t);
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
  if(destructor == NULL)
    {
      default_destructor(c);
    }
  else{
     destructor(c);
  }
  size_t temp = cascade_limit;
  remove_this_link(objectInfo);
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



objectInfo_t *find_previous_linkk(objectInfo_t *this_link){
 
  // detta är dumt (skriv i deviation)
  objectInfo_t *search = first_info;
  while(search->next != this_link && search->next != NULL){

    search = search->next;
  }

  if(search->next == NULL){
    return this_link;
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
  objectInfo_t *prev = find_previous_linkk(info);
  if(info == last_info){
    prev->next = NULL;
    last_info = prev;
    return;
  }
  
  prev->next = info->next;
}




void remove_next_link(objectInfo_t *trav){
  objectInfo_t *to_remove_inf = trav->next;
  trav->next = trav->next->next;
  

   long long to_remove_inf_adr = (long long) to_remove_inf;  // xd
   obj *to_remove = ((void *) to_remove_inf_adr + sizeof(objectInfo_t));

   to_remove_inf->func(to_remove);
   free(to_remove_inf);
   

}

void cleanup(){

  objectInfo_t *trav = first_info;
  
  if(trav == NULL)
    {
      return;
    }
  
  while(trav->next != NULL) 
    {
      
      if(trav->next->rf == 0){
      remove_next_link(trav);
      
    }
      
      if(trav->next != NULL)
        {
      trav = trav->next;
        }
  }


  
  if(first_info->rf == 0){
    
    objectInfo_t *new_first = first_info->next;
  

     long long first_info_adr = (long long) first_info; // detta e så facking sjukt

     obj *first_obj = ( (void *) first_info_adr + sizeof(objectInfo_t));

     //mer eller mindre en deallocate, fast just nu struntar vi i cascade_limit
     first_info->func(first_obj);
     free(first_info);
    
     
    
    first_info = new_first;
    
  }
}

void shutdown()
{
  objectInfo_t *current_info = first_info;

  while(current_info->next != NULL)
    {
      objectInfo_t *next_info = current_info->next;
      long long current_info_adr = (long long) current_info;
      if(next_info->next==NULL)
	{
	  deallocate( (void *) current_info_adr + sizeof(objectInfo_t));
	  break;
	}
      else{
	deallocate( (void *) current_info_adr + sizeof(objectInfo_t));
      
	current_info = next_info;
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
  objectInfo_t *objectInfo = c - sizeof(objectInfo_t);
  return objectInfo->rf;
}




