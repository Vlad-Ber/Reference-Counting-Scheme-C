#ifndef _COMMON_H
#define _COMMON_H
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define Success(v) (option_t) { .success = true, .element = v}
#define Failure()  (option_t) { .success = false }
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)

typedef union elem elem_t;
union elem
{
    int int_value;
    unsigned int uint_value;
    bool bool_value;
    float float_value;
    char *str_value;
    void *p;
};


typedef struct option option_t;
struct option
{
    bool success;
    elem_t element;
};

typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);
typedef bool(*ioopm_predicate)(elem_t a, elem_t b, void *extra);
typedef struct iter ioopm_list_iterator_t;

#endif
