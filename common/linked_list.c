#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

linked_list_t list_init() {
  return (linked_list_t){NULL, NULL};
}

void list_add(linked_list_t *list, char *name, long value) {
  list_node_t *ptr = malloc(sizeof(list_node_t));
  ptr->name = (char*)malloc(sizeof(char) * strlen(name));
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

/*TODO: May or may not need purge list with free*/