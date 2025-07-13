#include "../include/utils.h"

#include <stdlib.h>
#include <string.h>

char *clone_string(char *orig) {
  char *result = malloc(strlen(orig) + 1);
  strcpy(result, orig);
  return result;
}

char *join_strings(char *a, char *b) {
  char *result = malloc(strlen(a) + strlen(b) + 1);
  strcpy(result, a);
  strcat(result, b);
  return result;
}
