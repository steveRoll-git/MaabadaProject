#include "growing_array.h"

#include <stdio.h>
#include <stdlib.h>

growing_array_t growing_array_create() {
  return (growing_array_t) {NULL, 0, 0};
}

void growing_array_add(growing_array_t *array, array_element_t element) {
  if (array->count == array->capacity) {
    array_element_t *new_ptr;

    /* The new capacity will be 4 if there were no elements added until now, otherwise it'll be twice the current
     * capacity. */
    array->capacity = array->capacity == 0 ? 4 : array->capacity * 2;
    new_ptr = realloc(array->ptr, array->capacity * sizeof(array_element_t));

    if (new_ptr == NULL) {
      printf("Error: Could not allocate more memory for the array.\n");
      exit(EXIT_FAILURE);
    }

    array->ptr = new_ptr;
  }

  array->ptr[array->count] = element;
  array->count++;
}

void growing_array_free(growing_array_t *array) {
  free(array->ptr);
}
