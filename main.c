#include <stdio.h>
#include <stdlib.h>

#include "include/assembler.h"
#include "include/codegen.h"
#include "include/output.h"
#include "include/preprocessor.h"
#include "include/utils.h"

/* The file extension for input source files. */
#define EXTENSION_AS ".as"
/* The file extension for files after preprocessing. */
#define EXTENSION_AM ".am"
/* The file extension for object files. */
#define EXTENSION_OB ".ob"
/* The file extension for "entries" files. */
#define EXTENSION_ENT ".ent"
/* The file extension for "externals" files. */
#define EXTENSION_EXT ".ext"

/* A macro specifically for use in the `assemble_file` function. */
/* Evaluates the given expression. If the result is not successful, prints an error message, and jumps to the `end`
 * label to clean up memory. */
#define ASSEMBLE_TRY(f)                                                                                                \
  do {                                                                                                                 \
    result_t _result = (f);                                                                                            \
    if (_result != SUCCESS) {                                                                                          \
      printf("Error: %s\n", _result);                                                                                  \
      success = FALSE;                                                                                                 \
      goto end;                                                                                                        \
    }                                                                                                                  \
  }                                                                                                                    \
  while (0);

bool_t assemble_file(char *file_name) {
  bool_t success = TRUE;
  char *input_file_path = NULL;
  char *processed_path = NULL;
  char *object_path = NULL;
  char *entries_path = NULL;
  char *externals_path = NULL;
  /* This table is shared between the preprocess and codegen phases, to check that labels and macros don't mix. */
  table_t *macro_table = NULL;
  assembler_t *assembler = NULL;
  result_t result = SUCCESS;

  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_AS, &input_file_path))
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_AM, &processed_path))
  ASSEMBLE_TRY(table_create(sizeof(long), &macro_table))

  printf("Preprocessing file %s...\n", input_file_path);

  /* First, we run the file through the preprocessor which outputs a .am file. */
  ASSEMBLE_TRY(preprocess(input_file_path, processed_path, macro_table))

  ASSEMBLE_TRY(assembler_create(processed_path, macro_table, &assembler))
  printf("Generating code for file %s...\n", processed_path);

  /* If preprocessing succeeded, we generate the code for all instructions and directives. */
  ASSEMBLE_TRY(codegen(assembler))

  /* After successful code generation, we correct all the data labels so that they will point to the correct address in
   * the data image, after adding the value of IC to them. */
  merge_data(assembler);

  /* After all the labels have the correct values, we insert their values into all words that reference them. */
  /* Labels whose definitions were not found are caught here. */
  if (!resolve_labels(assembler)) {
    success = FALSE;
    goto end;
  }

  printf("Outputting object file...\n");
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_OB, &object_path))
  ASSEMBLE_TRY(output_object(assembler, object_path))

  printf("Outputting entries and externals files...\n");
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_ENT, &entries_path))
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_EXT, &externals_path))
  ASSEMBLE_TRY(output_entries_externals(assembler, entries_path, externals_path))

  printf("File assembled successfully.\n");

end:
  free(input_file_path);
  free(processed_path);
  free(object_path);
  free(entries_path);
  free(externals_path);
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
