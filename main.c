#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes/linked_list.h"
#include "firstpass/parser.h"
#include "preprocess/macroprocessor.h"

#define EXTENSION_AS ".as"
#define EXTENSION_AM ".am"

int main(int argc, char *argv[]) {
  int success;

  if (argc < 2) {
    printf("Usage %s input_file [output_file]\n", argv[0]);
    return EXIT_FAILURE;
  }

  /* TODO accept multiple files */
  char *file_name = argv[1];
  /* The input file's path is the name given as an argument, plus the ".as" extension. */
  char *input_file_path = malloc(strlen(file_name) + strlen(EXTENSION_AS) + 1);
  strcpy(input_file_path, file_name);
  strcat(input_file_path, EXTENSION_AS);

  char *preprocessor_output_path = malloc(strlen(file_name) + strlen(EXTENSION_AM) + 1);
  strcpy(preprocessor_output_path, file_name);
  strcat(preprocessor_output_path, EXTENSION_AM);

  success = preprocess(input_file_path, preprocessor_output_path);

  free(input_file_path);
  free(preprocessor_output_path);


  return success;
}
