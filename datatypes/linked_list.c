#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/data.h"
#include "assembler.h"

linked_list_t list_init() {
  return (linked_list_t) {NULL, NULL};
}

void llist_add(linked_list_t *list, char *name, long value) {
  list_node_t *ptr = malloc(sizeof(list_node_t));
  ptr->name = (char *) malloc(sizeof(char) * strlen(name));
  strcpy(ptr->name, name);
  ptr->value = value;
  ptr->next = NULL;

  if (list->head == NULL) {
    list->head = ptr;
    list->tail = ptr;
    return;
  }

  list->tail->next = ptr;
  list->tail = ptr;
}

long list_get(linked_list_t *list, char *name) {
  list_node_t *tmp = list->head;
  while (tmp != NULL) {
    if (strcmp(name, tmp->name) == 0) {
      return tmp->value;
    }
    tmp = tmp->next;
  }
  return -1L;
}

void print_list(linked_list_t *list) {
  list_node_t *temp = list->head;
  while (temp != NULL) {
    printf(" [\"%s\", %lu ] ->", temp->name, temp->value);
    temp = temp->next;
  }
  printf("\n");
}

int list_size(linked_list_t *list) {
  if (list == NULL) {
    return 0;
  }
  const list_node_t *tmp = list->head;
  int size = 0;
  /*FIXME: what do i do here, INIT fucks me*/
  if (tmp == NULL) {
    return 0;
  }

  while (tmp != NULL) {
    size++;
    tmp = tmp->next;
  }
  return size;
}


int *list_items(linked_list_t *list) {
  if (list == NULL) {
    return NULL;
  }

  const int size = list_size(list);
  const list_node_t *tmp = list->head;
  int i = 0;

  int *items = (int *) malloc(sizeof(int) * size);
  if (items == NULL) {
    return NULL;
  }

  for (i = 0; i < size; i++) {
    items[i] = tmp->value;
    tmp = tmp->next;
  }

  return items;
}
void purge_list(linked_list_t *list) {
  if (list == NULL) {
    return;
  }

  list_node_t *node = list->head;
  list_node_t *tmp = list->head;
  char *str;

  while (node != NULL) {
    tmp = node;
    tmp->next = NULL;
    node = node->next;

    str = tmp->name;
    free(tmp);
    free(str);
  }

  list->head = NULL;
  list->tail = NULL;
}

/*TODO: May or may not need purge list with free*/
