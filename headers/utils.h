#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#include "errors.h"

/* Given two integers, returns the smaller of the two. */
int min(int a, int b);

/* Runs `malloc` with the given size, and checks that it ran successfully (returned a non-null pointer). */
/* May fail if allocating memory did not succeed. */
result_t check_malloc(size_t size, void **ptr);

/* This macro accepts a variable that is a pointer to a pointer to a non-void type. */
/* It runs `check_malloc` with the size of the pointer's underlying type and stores the new pointer there. */
/* If allocation fails, causes the running function to return an "out of memory" error. */
#define TRY_MALLOC(ptr) TRY(check_malloc(sizeof(**ptr), (void **) ptr))

/* Creates a copy of the given string, and stores its pointer in `out`. */
/* The resulting pointer must be freed when it is no longer needed. */
/* May fail if memory allocations did not succeed. */
result_t clone_string(char *orig, char **out);

/* Creates a new string whose value is strings `a` and `b` joined together, and stores its pointer in `out`. */
/* The resulting pointer must be freed when it is no longer needed. */
/* May fail if memory allocations did not succeed. */
result_t join_strings(char *a, char *b, char **out);

#endif
