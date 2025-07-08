#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

/* An array that can dynamically grow as more elements are added to it. */
typedef struct array_t {
  /* The pointer to the data. It may change after the array grows. */
  void *ptr;
  /* The size of the element type that this array stores. */
  size_t element_size;
  /* The current number of elements in the array. */
  size_t count;
  /* The current maximum number of elements this array can hold. */
  size_t capacity;
} array_t;

/* Creates and returns a new growing array with the specified element size. It will have an initial capacity of 0. */
array_t array_create(size_t element_size);

/* Makes sure that the growing array can store one more element, and returns a pointer to the last element. */
/* The ARRAY_ADD macro can be used for convenience. */
void *array_add(array_t *array);

/* Assumes that the array was created with the same element size as the given element. */
/* Uses array_add to allocate space for the new element, and adds it into that space. */
#define ARRAY_ADD(array, element) *((typeof(element) *) array_add(array)) = (element);

/* Frees the memory used by this array. */
void array_free(array_t *array);

#endif
