#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdio.h>
#include "../common/data.h"
/* A node in a linked list. */
typedef struct list_node_t {
  char *name;
  long value;
  struct list_node_t *next;
} list_node_t;

/* A linked list, that stores references to its head and tail nodes. */
typedef struct linked_list_t {
  list_node_t *head;
  list_node_t *tail;
} linked_list_t;

/* Adds a new macro into our linkedlist*/
void llist_add(linked_list_t *list, char *name, long value);

/* Checks if the list contains a node with the given name - if it does, returns that node's value, otherwise returns -1.
 */
long list_get(linked_list_t *list, char *name);

/**/
void print_list(linked_list_t *list);

/* Initializes a new list. */
linked_list_t list_init();


int list_length(linked_list_t *list);

int *list_items(linked_list_t *list);

void purge_list(linked_list_t *list);

#endif
