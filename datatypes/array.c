#include "array.h"

#include <stdio.h>
#include <stdlib.h>

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

size_t array_count(array_t *array) {
  return array->count;
}

array_t *array_create(size_t element_size) {
  array_t *array = malloc(sizeof(array_t));
  array->ptr = NULL;
  array->element_size = element_size;
  array->count = 0;
  array->capacity = 0;
  return array;
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
  free(array);
}
