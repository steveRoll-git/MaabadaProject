#include "datatypes.h"
#include <stdio.h>
#include <string.h>

int llm_add(llm_t *head, char *name, long offset) {
  int size = strlen(name);

  char *name_dup = (char *)malloc(size);

  strncpy(name_dup, name, size);
  if (head->next == NULL) {
    head->next = (llm_t *)malloc(sizeof(llm_t));
    head->next->macro.name = name_dup;
    head->next->macro.index = offset;
    return 1;
  }

  else {
    llm_t *temp = head->next;
    head->next = (llm_t *)malloc(sizeof(llm_t));
    head->next->macro.index = offset;
    head->next->macro.name = name_dup;
    head->next->next = temp;
    return 1;
  }
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
