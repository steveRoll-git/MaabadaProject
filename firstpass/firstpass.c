#include <stdio.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "parser.h"
void compile_line(char *line) {
}

/* TODO temp arguments */
int first_pass(char *input_file_path, char *output_file_path) {
  assembler_t assembler = assembler_create();
  FILE *in, *out;
  char line[MAX_LINE];

  in = fopen(input_file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    printf("Couldn't open input file\n");
    return 0;
  }
  /*TODO: Initialize Status Error Value Here.*/
  while (read_line(in, line) != SENTENCE_EOF) {
    compile_assembly_code(line, &assembler);
  }

  /*TODO: check if error status code has been lit, if it has DO NOT go into secondpass and delete all files
   * (optionally?) [It might be better if MAIN crashes the program, and firstpass tell its ERROR status] */


  /*TODO: Merge between DC and IC into label_table.*/
}
