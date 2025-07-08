#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

/* An array that can dynamically grow as more elements are added to it. */
typedef struct array_t array_t;

/* Returns the number of elements in the array. */
size_t array_count(array_t *array);

/* Creates and returns a new growing array with the specified element size. It will have an initial capacity of 0. */
array_t *array_create(size_t element_size);

/* Returns a pointer to the element at the given index in this array. */
void *array_at(array_t *array, int index);

/* Makes sure that the growing array can store one more element, and returns a pointer to the last element. */
/* The ARRAY_ADD macro can be used for convenience. */
void *array_add(array_t *array);

/* Assumes that the array was created with the same element size as the given element's size. */
/* Uses array_add to allocate space for the new element, and adds it into that space. */
#define ARRAY_ADD(array, element) *((typeof(element) *) array_add(array)) = (element);

/* Frees the memory used by this array, and the array itself. */
/* (This does not free any pointers that the array elements may have.) */
void array_free(array_t *array);

#endif
