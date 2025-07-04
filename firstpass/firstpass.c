#include <stdio.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "parser.h"
void compile_line(char *line) {
}

/* TODO temp arguments */
int first_pass(char *input_file_path, char *output_file_path, assembler_t *assembler) {

  FILE *in, *out;
  char line[MAX_LINE];
  int line_number = 0, error_flag = FALSE, res, total_errors = 0;


  in = fopen(input_file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    printf("Couldn't open input file\n");
    return 0;
  }

  while (read_line(in, line) != SENTENCE_EOF) {
    res = compile_assembly_code(line, assembler);
    line_number++;

    if (!res) {
      fprintf(stderr, " : Line %d \n", line_number);
      error_flag = TRUE;
      total_errors++;
    }
  }

  merge_data(assembler);
  /*TODO: Merge between DC and IC into label_table.*/

  // merge_dc_to_ic()
  printf("Total Errors: %d\n", total_errors);
  return error_flag;
}
