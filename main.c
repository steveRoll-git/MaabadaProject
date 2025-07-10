#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/preprocessor.h"

/* The file extension for input source files. */
#define EXTENSION_AS ".as"
/* The file extension for files after preprocessing. */
#define EXTENSION_AM ".am"

/* Creates a new string whose value is strings `a` and `b` joined together. */
/* The returned pointer must be freed when it is no longer needed. */
char *join_strings(char *a, char *b) {
  char *result = malloc(strlen(a) + strlen(b) + 1);
  strcpy(result, a);
  strcat(result, b);
  return result;
}

int main(int argc, char *argv[]) {
  int success;
  if (argc < 2) {
    printf("Usage %s input_file [output_file]\n", argv[0]);
    return EXIT_FAILURE;
  }
  /* TODO accept multiple files */
  char *file_name = argv[1];
  /* The input file's path is the name given as an argument, plus the ".as" extension. */
  char *input_file_path = join_strings(file_name, EXTENSION_AS);
  char *preprocessor_output_path = join_strings(file_name, EXTENSION_AM);

  success = preprocess(input_file_path, preprocessor_output_path);

  free(input_file_path);
  free(preprocessor_output_path);

  return success;
}
