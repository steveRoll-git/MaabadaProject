#include "datatypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

llm_t *llm_init() {
  llm_t *head = (llm_t *)malloc(sizeof(llm_t));
  head->macro.index = 0;
  head->macro.name = "";
  return head;
}

int llm_add(llm_t *head, char *name, long offset) {
  int size = strlen(name);
  llm_t *temp;

  if (!strcmp(head->macro.name, "")) {
    head->macro.name = name;
    head->macro.index = offset;
    return 1;
  }

  llm_t *ptr = (llm_t *)malloc(sizeof(llm_t));

  ptr->macro.index = offset;
  ptr->macro.name = name;

  temp = head->next;
  head->next = ptr;
  ptr->next = temp;

  return 1;
}

long llm_contains(llm_t *head, char *name) {

  llm_t *tmp = head;
  while (tmp != NULL) {
    if (!strcmp(name, tmp->macro.name)) {
      return tmp->macro.index;
    }
    tmp = tmp->next;
  }
  return -1L;
}

void print_llm(llm_t *head) {
  llm_t *temp = head;
  while (temp != NULL) {
    printf(" [\"%s\", %lu ] ->", temp->macro.name, temp->macro.index);
    temp = temp->next;
  }
  printf("\n");
}
