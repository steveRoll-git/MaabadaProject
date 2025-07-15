#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#include "errors.h"

/* Runs `malloc` with the given size, and checks that it ran successfully (returned a non-null pointer). */
/* If not, returns an error. */
result_t check_malloc(size_t size, void **ptr);

/* This macro accepts a variable that is a pointer to a pointer to a non-void type. */
/* It runs `check_malloc` with the size of the pointer's underlying type and stores the new pointer there. */
/* If allocation fails, causes the running function to return an "out of memory" error. */
#define TRY_MALLOC(ptr) TRY(check_malloc(sizeof(**ptr), (void **) ptr))

/* Creates a copy of the given string, and stores its pointer in `out`. */
/* The resulting pointer must be freed when it is no longer needed. */
result_t clone_string(char *orig, char **out);

/* Creates a new string whose value is strings `a` and `b` joined together, and stores its pointer in `out`. */
/* The resulting pointer must be freed when it is no longer needed. */
result_t join_strings(char *a, char *b, char **out);

#endif
