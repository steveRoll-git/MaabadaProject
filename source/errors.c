#include "../include/errors.h"

#include <stdio.h>

void print_error(char *file_name, int line_number, char *message) {
  printf("Error at %s:%d: %s\n", file_name, line_number, message);
}
