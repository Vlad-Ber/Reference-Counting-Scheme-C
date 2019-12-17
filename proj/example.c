#include "refmem.c"
#include <assert.h>

struct cell
{
  struct cell *cell;
  int i;
  char *string;
};

void cell_destructor(obj *c)
{
  release(((struct cell *) c)->cell);
}


int main()
{
  set_cascade_limit(10);
  struct cell *c = allocate(sizeof(struct cell), cell_destructor);
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
