#include "../include/utils.h"

#include <stdlib.h>
#include <string.h>

result_t check_malloc(size_t size, void **ptr) {
  *ptr = malloc(size);
  ASSERT(*ptr, ERR_OUT_OF_MEMORY)
  return SUCCESS;
}

result_t clone_string(char *orig, char **out) {
  TRY(check_malloc(strlen(orig) + 1, (void **) out))
  strcpy(*out, orig);
  return SUCCESS;
}

result_t join_strings(char *a, char *b, char **out) {
  TRY(check_malloc(strlen(a) + strlen(b) + 1, (void **) out))
  strcpy(*out, a);
  strcat(*out, b);
  return SUCCESS;
}
