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
  struct cell *c = allocate(sizeof(struct cell), cell_destructor);
  //int *c = NULL; //får segfault
  printf("rc = %zu\n",rc(c));
  assert(rc(c) == 0);
  retain(c);
  printf("rc = %zu\n",rc(c));
  assert(rc(c) == 1);
  
  c->cell = allocate(sizeof(struct cell), cell_destructor);
  assert(rc(c->cell) == 0);
  retain(c->cell);
  assert(rc(c->cell) == 1);
  
  c->cell->cell = NULL;
  
  release(c);
 

  return 0;
}
