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
  /* This table is shared between the preprocess and codegen phases, to check that labels and macros don't mix. */
  table_t *macro_table = table_create(sizeof(long));
  assembler_t *assembler = NULL;

  printf("Preprocessing file %s...\n", input_file_path);

  /* First, we run the file through the preprocessor which outputs a .am file. */
  if (!preprocess(input_file_path, processed_path, macro_table)) {
    success = FALSE;
    goto end;
  }

  assembler = assembler_create(macro_table);
  printf("Generating code for file %s...\n", processed_path);

  /* If preprocessing succeeded, we generate the code for all instructions and directives. */
  if (!codegen(processed_path, assembler)) {
    success = FALSE;
    goto end;
  }

  /* After successful code generation, we correct all the data labels so that they will point to the correct address in
   * the data image, after adding the value of IC to them. */
  merge_data(assembler);

  /* After all the labels have the correct values, we insert their values into all words that reference them. */
  /* Labels whose definitions were not found are caught here. */
  if (!resolve_labels(assembler, processed_path)) {
    success = FALSE;
    goto end;
  }

end:
  free(input_file_path);
  free(processed_path);
  table_free(macro_table);
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
