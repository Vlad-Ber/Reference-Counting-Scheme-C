#include "refmem.c"
#include <assert.h>


struct shell
{
    struct shell *shell;
    //  char *string;
    int k;
    //int s;
    //size_t mitocondria;
    //int i;
    //char *string2;
};

struct cell
{
    struct cell *cell;

    char *string;
    int k;
    int s;
    size_t mitocondria;
    int i;
    char *string2;
};

void cell_destructor(obj *c)
{
    printf("cell destruct \n");
    release(((struct cell *) c)->cell);
}


int main()
{

    printf("cell size: %ld\n", sizeof(struct cell));
    printf("sizeof objectinfo_t: %ld\n", sizeof(objectInfo_t));
    printf("sizeof objectinfo_t*: %ld\n", sizeof(objectInfo_t*));
    printf("sizeof size_t: %ld\n", sizeof(size_t));
    printf("sizeof function1_t: %ld\n", sizeof(function1_t));



    set_cascade_limit(10);
    struct cell *c = allocate(sizeof(struct cell), cell_destructor);

    c->string = "hejsan";
    c->i = 11;
    c->k = 17;
    //printf("c_vals: %s, %d, %d\n", c->string, c->i, c->k);


    printf("rc = %zu\n",rc(c));
    //assert(rc(c) == 0);
    retain(c);
    printf("rc = %zu\n",rc(c));
    //assert(rc(c) == 1);
    get_cascade_limit(); //ta bort
    struct cell *l;
    c->cell = allocate(sizeof(struct cell), cell_destructor);
    l = c->cell;

    l->string = "hejsan";
    l->i = 11;
    l->k = 17;
    //printf("l_vals: %s, %d, %d\n", l->string, l->i, l->k);


    //assert(rc(c->cell) == 0);
    retain(c->cell);
    //assert(rc(c->cell) == 1);

    c->cell->cell = allocate(sizeof(struct cell), cell_destructor);

    c->cell->cell->cell = allocate(sizeof(struct cell), cell_destructor);

    retain(c->cell->cell);

    c->cell->cell->cell->cell=NULL;
    release(c);
    printf("casdadelimit in the end is %ld\n \n \n", cascade_limit);



    struct cell *cell = allocate(sizeof(struct cell), cell_destructor);
    struct cell *cell2 = allocate(sizeof(struct cell), cell_destructor);

    cell->i = 2;
    cell2->i = 2;

    shutdown();

    struct shell *shell1 = allocate(sizeof(struct shell), NULL);
    shell1->shell = allocate(sizeof(struct shell), NULL);
    shell1->shell->shell = allocate(sizeof(struct shell), NULL);
    shell1->shell->shell->shell = allocate(sizeof(struct shell), NULL);
    shell1->shell->shell->shell->shell = allocate(sizeof(struct shell), NULL);
    deallocate(shell1);

    //printf("sizeof shell: %ld \n",sizeof(struct shell));
    //cleanup();
    //cleanup();


    struct cell  *arr1 = allocate_array(2, sizeof(struct cell),NULL );
    retain(arr1);

    objectInfo_t *arr1_info = (void *) (long long) arr1-sizeof(objectInfo_t);

    struct cell *cello1 = arr1;

    struct cell *cello2 = arr1 +1;
    cello2->k = 15;



    size_t size_cell =   sizeof(struct cell);

    struct cell *cello2_maybe = (void *) (long long) arr1 + size_cell;

    printf("k val: %d \n ", cello2_maybe->k);
    printf("arr1 rc: %ld\n", arr1_info->rf);

    release(arr1);



    return 0;
}
