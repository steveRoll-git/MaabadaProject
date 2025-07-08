#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/* A list that can dynamically grow as more elements are added to it. */
typedef struct list_t list_t;

/* Creates and returns a new list with the specified element size. It will have an initial capacity of 0. */
list_t *list_create(size_t element_size);

/* Returns the number of elements in the list. */
size_t list_count(list_t *list);

/* Returns a pointer to the element at the given index. */
void *list_at(list_t *list, int index);

/* Makes sure that the list can store one more element, and returns a pointer to the last element. */
/* The LIST_ADD macro can be used for convenience. */
void *list_add(list_t *list);

/* Assumes that the list was created with the same element size as the given element's size. */
/* Uses list_add to allocate space for the new element, and adds it into that space. */
#define LIST_ADD(list, element) *((typeof(element) *) list_add(list)) = (element);

/* Frees the memory used by this list, and the list itself. */
/* (This does not free any pointers that the list elements may have.) */
void list_free(list_t *list);

#endif
