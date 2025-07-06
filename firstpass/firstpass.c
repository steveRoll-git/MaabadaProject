#include <stdio.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "parser.h"

machine_word_t make_code_word(int opcode, int src_operand, int dst_operand, int era) {
}

void write_instruction(assembler_t *assembler, instruction_t *instruction) {
  add_code_word(assembler, instruction->info->opcode);
}

void write_statement(assembler_t *assembler, statement_t *statement) {
  if (statement->kind == STATEMENT_INSTRUCTION) {
  }
}

/* TODO temp arguments */
int first_pass(char *input_file_path, char *output_file_path, assembler_t *assembler) {
  FILE *in, *out;
  char line[MAX_LINE];
  int line_number = 1, error_flag = FALSE, total_errors = 0;

  in = fopen(input_file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    printf("Couldn't open input file\n");
    return 0;
  }

  while (read_line(in, line) != SENTENCE_EOF) {
    statement_t statement;
    bool_t res = parse_statement(line, &statement);

    if (!res) {
      fprintf(stderr, " : Line %d in %s \n", line_number, input_file_path);
      error_flag = TRUE;
      total_errors++;
    }
    else if (statement.kind != STATEMENT_EMPTY) {
      if (statement.has_label) {
        if (statement.kind == STATEMENT_INSTRUCTION) {
        }
      }
    }

    line_number++;
  }

  merge_data(assembler);
  /*TODO: Merge between DC and IC into label_table.*/

  // merge_dc_to_ic()
  printf("Total Errors: %d\n", total_errors);
  return error_flag;
}
