#include "../include/list.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/utils.h"

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
  return (void *) ((size_t) list->ptr + index * list->element_size);
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

  *out = (void *) ((size_t) list->ptr + (list->count * list->element_size));
  list->count++;

  return SUCCESS;
}

void list_free(list_t *list) {
  if (!list) {
    return;
  }
  free(list->ptr);
  free(list);
}
