#include <stdio.h>
#include <stdlib.h>

#include "../headers/codegen.h"
#include "../headers/context.h"
#include "../headers/output.h"
#include "../headers/preprocessor.h"
#include "../headers/utils.h"

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

/**
 * A macro specifically for use in the `assemble_file` function.
 * Evaluates the given expression, which must return a `result_t`. If the result is not successful, sets the `result`
 * variable to the returned error code, and jumps to the `cleanup` label to clean up memory.
 *
 * @param f The expression to evaluate. Must be of a `result_t` type.
 */
#define ASSEMBLE_TRY(f)                                                                                                \
  {                                                                                                                    \
    result_t _result = (f);                                                                                            \
    if (_result != SUCCESS) {                                                                                          \
      result = _result;                                                                                                \
      goto cleanup;                                                                                                    \
    }                                                                                                                  \
  }

/**
 * Performs all stages of the assembler on the file at the given path:
 * Preprocessing, code generation, resolving labels, and outputting files (object, entries, externals).
 * The given file name must be the name of an existing assembly file, without the ".as" extension.
 * If any of the stages fail, or if memory allocation failed at any point, returns the relevant error.
 *
 * @param file_name The name of the input file - the path to the .as file, but without the ".as" extension.
 * @return The operation's result.
 */
result_t assemble_file(char *file_name) {
  /* The result that this function will return when it finishes. */
  result_t result = SUCCESS;
  /* The path to the .as file. */
  char *input_file_path = NULL;
  /* The path to the .am file. */
  char *processed_path = NULL;
  /* The path to the .ob file. */
  char *object_path = NULL;
  /* The path to the .ent file. */
  char *entries_path = NULL;
  /* The path to the .ext file. */
  char *externals_path = NULL;
  /* This table is shared between the preprocess and codegen phases, to check that labels and macros don't mix. */
  table_t *macro_table = NULL;
  /* The assembler context, created after preprocessing. */
  context_t *context = NULL;

  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_AS, &input_file_path))
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_AM, &processed_path))
  ASSEMBLE_TRY(table_create(sizeof(long), &macro_table))

  printf("Preprocessing file %s...\n", input_file_path);

  /* First, we run the file through the preprocessor which outputs a .am file. */
  ASSEMBLE_TRY(preprocess(input_file_path, processed_path, macro_table))

  ASSEMBLE_TRY(context_create(processed_path, macro_table, &context))
  printf("Generating code for file %s...\n", processed_path);

  /* If preprocessing succeeded, we generate the code for all instructions and directives. */
  ASSEMBLE_TRY(codegen(context))

  /* After successful code generation, we correct all the data labels so that they will point to the correct address in
   * the data image, after adding the value of IC to them. */
  merge_data(context);

  printf("Resolving labels...\n");

  /* After all the labels have the correct values, we insert their values into all words that reference them. */
  /* Labels whose definitions were not found are caught here. */
  ASSEMBLE_TRY(resolve_labels(context))

  /* If all the steps above succeeded, we finally output the object, entries and externals files. */

  printf("Outputting object file...\n");
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_OB, &object_path))
  ASSEMBLE_TRY(output_object(context, object_path))

  printf("Outputting entries and externals files...\n");
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_ENT, &entries_path))
  ASSEMBLE_TRY(join_strings(file_name, EXTENSION_EXT, &externals_path))
  ASSEMBLE_TRY(output_entries_externals(context, entries_path, externals_path))

  printf("File assembled successfully.\n\n");

cleanup:
  /* Free all the memory we allocated. */
  free(input_file_path);
  free(processed_path);
  free(object_path);
  free(entries_path);
  free(externals_path);
  table_free(macro_table);
  context_free(context);

  if (result != SUCCESS) {
    printf("Could not assemble file: ");
    print_error(result);
    printf("\n\n");
  }

  return result;
}

/* The assembler's main function iterates over all given file paths and calls `assemble_file` on them. */
/* If an "out of memory" was encountered at any point, the program will stop and no more files will be assembled. */
int main(int argc, char *argv[]) {
  int i;
  bool_t success = TRUE;

  if (argc < 2) {
    printf("No input files given.\n");
    return EXIT_SUCCESS;
  }

  for (i = 1; i < argc; i++) {
    char *file_name = argv[i];
    result_t result = assemble_file(file_name);

    if (result == ERR_OUT_OF_MEMORY) {
      /* If we ran out of memory, we won't be able to process any more files. */
      return EXIT_FAILURE;
    }

    if (result != SUCCESS) {
      success = FALSE;
    }
  }

  if (!success) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
