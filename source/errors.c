#include "../include/errors.h"

#include <stdio.h>

void print_error(char *file_path, int line_number, char *message) {
  printf("Error at %s:%d: %s\n", file_path, line_number, message);
}
