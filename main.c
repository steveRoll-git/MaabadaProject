#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/assembler.h"
#include "include/codegen.h"
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

bool_t assemble_file(char *file_name) {
  bool_t success = TRUE;
  /* The input file's path is the name given as an argument, plus the ".as" extension. */
  char *input_file_path = join_strings(file_name, EXTENSION_AS);
  char *processed_path = join_strings(file_name, EXTENSION_AM);
  assembler_t *assembler = NULL;

  printf("Preprocessing file %s...\n", input_file_path);

  if (!preprocess(input_file_path, processed_path)) {
    success = FALSE;
    goto end;
  }

  assembler = assembler_create();
  printf("Assembling file %s...\n", processed_path);

  if (!codegen(processed_path, assembler)) {
    success = FALSE;
    goto end;
  }

  merge_data(assembler);

end:
  free(input_file_path);
  free(processed_path);
  assembler_free(assembler);

  return success;
}

int main(int argc, char *argv[]) {
  bool_t success;
  if (argc < 2) {
    printf("Usage %s input_file [output_file]\n", argv[0]);
    return EXIT_FAILURE;
  }
  /* TODO accept multiple files */
  char *file_name = argv[1];

  success = assemble_file(file_name);

  if (!success) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
