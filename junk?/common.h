#pragma once
#include <stdbool.h>
#include <stdlib.h>


typedef union elem elem_t;

union elem
{
    /// TODO: update the names of the fields to something better?
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
    /// other choices certainly possible
};

// TODO: We have assumed that return type would be int. What consequenses does it have?
typedef bool(*cmp_fun_t)(elem_t a, elem_t b);
typedef size_t(*hash_fun_t)(elem_t key);
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

// f a b = a >= b
//
// f x y == f y x
