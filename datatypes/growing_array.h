#ifndef GROWING_ARRAY_H
#define GROWING_ARRAY_H

#include <stddef.h>

/* The type of element that the growing array will store. */
typedef int array_element_t;

/* An array of `int`s that can dynamically grow as more elements are added to it. */
typedef struct growing_array_t {
  /* The pointer to the data. It may change after the array grows. */
  array_element_t *ptr;
  /* The current number of elements in the array. */
  size_t count;
  /* The current maximum number of elements this array can hold. */
  size_t capacity;
} growing_array_t;

/* Creates and returns a new growing array, with an initial capacity of 0. */
growing_array_t growing_array_create();

/* Adds an element to the end of the array, reallocating it if necessary. */
/* If reallocation fails (usually because there's no memory left) an error will be printed and the program will exit. */
void growing_array_add(growing_array_t *array, array_element_t element);

/* Frees the memory used by this array. */
void growing_array_free(growing_array_t *array);

#endif
