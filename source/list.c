#include "../include/list.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/utils.h"

typedef struct list_t {
  /* The pointer to the data. It may change after the array grows. */
  void *ptr;
  /* The size of the element type that this array stores. */
  size_t element_size;
  /* The current number of elements in the array. */
  size_t count;
  /* The current maximum number of elements this array can hold. */
  size_t capacity;
} list_t;

result_t list_create(size_t element_size, list_t **list) {
  TRY_MALLOC(list)
  (*list)->ptr = NULL;
  (*list)->element_size = element_size;
  (*list)->count = 0;
  (*list)->capacity = 0;
  return SUCCESS;
}

size_t list_count(list_t *list) {
  return list->count;
}

void *list_at(list_t *list, int index) {
  return list->ptr + index * list->element_size;
}

result_t list_add(list_t *list, void **out) {
  if (list->count == list->capacity) {
    /* If adding one more element will cause the list's count to exceed its capacity, we increase the capacity and
     * reallocate the array. */
    void *new_ptr;

    /* The new capacity will be 4 if there were no elements added until now, otherwise it'll be twice the current
     * capacity. */
    list->capacity = list->capacity == 0 ? 4 : list->capacity * 2;
    new_ptr = realloc(list->ptr, list->capacity * list->element_size);

    /* If `new_ptr` is null, it means `realloc` failed. If this happens we return an "out of memory" error. */
    ASSERT(new_ptr, ERR_OUT_OF_MEMORY)

    list->ptr = new_ptr;
  }

  *out = list->ptr + (list->count++ * list->element_size);
  return SUCCESS;
}

void list_free(list_t *list) {
  if (!list) {
    return;
  }
  free(list->ptr);
  free(list);
}
