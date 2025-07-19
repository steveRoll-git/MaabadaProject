#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#include "errors.h"

/* A list that can dynamically grow as more elements are added to it. */
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

/* Creates a new list with the specified element size, and stores it in `list`.
 * It will have an initial capacity of 0. */
/* May fail if memory allocations did not succeed. */
result_t list_create(size_t element_size, list_t **list);

/* Returns the number of elements in the list. */
size_t list_count(list_t *list);

/* Returns a pointer to the element at the given index. */
void *list_at(list_t *list, int index);

/* Makes sure that the list can store one more element, and returns a pointer to the last element. */
/* The LIST_ADD macro can be used to also add an element in the same line. */
/* May fail if memory allocations did not succeed. */
result_t list_add(list_t *list, void **out);

/* Given a list_t, a type, and a value, inserts the value into the list, assuming both the list's value type and the
 * given value's type are the same. */
/* list_add may fail, so this must be called in a function that returns a `result_t`. */
#define LIST_ADD(list, element_type, element)                                                                          \
  {                                                                                                                    \
    void *_ptr;                                                                                                        \
    TRY(list_add(list, &_ptr))                                                                                         \
    *((element_type *) _ptr) = (element);                                                                              \
  }

/* Frees the memory used by this list, and the list itself. */
/* (This does not free any pointers that the list elements may have.) */
/* If the given pointer is NULL, nothing is performed. */
void list_free(list_t *list);

#endif
