#include "array.h"

#include <stdio.h>
#include <stdlib.h>

array_t array_create(size_t element_size) {
  return (array_t) {NULL, element_size, 0, 0};
}

void *array_at(array_t *array, int index) {
  return array->ptr + index * array->element_size;
}

void *array_add(array_t *array) {
  if (array->count == array->capacity) {
    /* If adding one more element will cause the array's count to exceed its capacity, we increase the capacity and
     * reallocate the array. */
    void *new_ptr;

    /* The new capacity will be 4 if there were no elements added until now, otherwise it'll be twice the current
     * capacity. */
    array->capacity = array->capacity == 0 ? 4 : array->capacity * 2;
    new_ptr = realloc(array->ptr, array->capacity * array->element_size);

    if (new_ptr == NULL) {
      printf("Error: Could not allocate more memory for the array.\n");
      exit(EXIT_FAILURE);
    }

    array->ptr = new_ptr;
  }

  return array->ptr + (array->count++ * array->element_size);
}

void array_free(array_t *array) {
  free(array->ptr);
}
