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

bool_t assemble_file(char *file_name) {
  bool_t success = TRUE;
  char *input_file_path = join_strings(file_name, EXTENSION_AS);
  char *processed_path = join_strings(file_name, EXTENSION_AM);
  char *object_path = NULL;
  char *entries_path = NULL;
  char *externals_path = NULL;
  /* This table is shared between the preprocess and codegen phases, to check that labels and macros don't mix. */
  table_t *macro_table = table_create(sizeof(long));
  assembler_t *assembler = NULL;
  result_t result = SUCCESS;

  printf("Preprocessing file %s...\n", input_file_path);

  /* First, we run the file through the preprocessor which outputs a .am file. */
  result = preprocess(input_file_path, processed_path, macro_table);
  if (result != SUCCESS) {
    printf("Preprocessing failed: %s\n", result);
    success = FALSE;
    goto end;
  }

  assembler = assembler_create(processed_path, macro_table);
  printf("Generating code for file %s...\n", processed_path);

  /* If preprocessing succeeded, we generate the code for all instructions and directives. */
  if (!codegen(assembler)) {
    success = FALSE;
    goto end;
  }

  /* After successful code generation, we correct all the data labels so that they will point to the correct address in
   * the data image, after adding the value of IC to them. */
  merge_data(assembler);

  /* After all the labels have the correct values, we insert their values into all words that reference them. */
  /* Labels whose definitions were not found are caught here. */
  if (!resolve_labels(assembler)) {
    success = FALSE;
    goto end;
  }

  object_path = join_strings(file_name, EXTENSION_OB);
  output_object(assembler, object_path);

  entries_path = join_strings(file_name, EXTENSION_ENT);
  externals_path = join_strings(file_name, EXTENSION_EXT);
  output_entries_externals(assembler, entries_path, externals_path);

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
