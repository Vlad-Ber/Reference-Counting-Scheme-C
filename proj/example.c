#include "refmem.c"
#include <assert.h>

struct cell
{
  struct cell *cell;
  int i;
  char *string;
  int k;
};

void cell_destructor(obj *c)
{
  printf("cell destruct");
  release(((struct cell *) c)->cell);
}


int main()
{

  printf("sizeof objectinfo_t: %ld\n", sizeof(objectInfo_t));
  printf("sizeof objectinfo_t*: %ld\n", sizeof(objectInfo_t*));
  printf("sizeof size_t: %ld\n", sizeof(size_t));
  printf("sizeof function1_t: %ld\n", sizeof(function1_t));

  
  
  set_cascade_limit(10);
  struct cell *c = allocate(sizeof(struct cell), cell_destructor);
  c->string = "hejsan";
  
  
  printf("rc = %zu\n",rc(c));
  assert(rc(c) == 0);
  retain(c);
  printf("rc = %zu\n",rc(c));
  assert(rc(c) == 1);
  get_cascade_limit(); //ta bort
  c->cell = allocate(sizeof(struct cell), cell_destructor);
  assert(rc(c->cell) == 0);
  retain(c->cell);
  assert(rc(c->cell) == 1);

  c->cell->cell = allocate(sizeof(struct cell), cell_destructor);

  c->cell->cell->cell = allocate(sizeof(struct cell), cell_destructor);
  
   retain(c->cell->cell);
   
   c->cell->cell->cell->cell=NULL;
   release(c);
   printf("casdadelimit in the end is %ld\n", cascade_limit);
  
  return 0;
}
