#include <stdio.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "parser.h"

#define OPCODE_FIRST_BIT 6
#define OPCODE_MASK 0xf
#define SRC_FIRST_BIT 4
#define SRC_MASK 0x3
#define DST_FIRST_BIT 2
#define DST_MASK 0x3
#define ROW_FIRST_BIT 6
#define COL_FIRST_BIT 2
#define REG_MASK 0xf
#define IMM_MASK 0xff

machine_word_t make_code_word(int opcode, int src_operand, int dst_operand, int era) {
  machine_word_t word = 0;
  word |= (opcode & OPCODE_MASK) << OPCODE_FIRST_BIT;
  word |= (src_operand & SRC_MASK) << SRC_FIRST_BIT;
  word |= (dst_operand & DST_MASK) << DST_FIRST_BIT;
  return word;
}

void write_operand(assembler_t *assembler, operand_t *operand) {
  switch (operand->kind) {
    case OPERAND_KIND_IMMEDIATE:
      add_code_word(assembler, (operand->data.immediate & IMM_MASK) << DST_FIRST_BIT);
      break;
    case OPERAND_KIND_LABEL:
      add_code_word(assembler, 0); /* TODO */
      break;
    case OPERAND_KIND_MATRIX:
      /* The first word in matrix addressing is the address of the label. */
      add_code_word(assembler, 0); /* TODO */
      add_code_word(assembler,
                    (operand->data.matrix.row_reg & REG_MASK) << ROW_FIRST_BIT |
                            (operand->data.matrix.col_reg & REG_MASK) << COL_FIRST_BIT);
      break;
    case OPERAND_KIND_REGISTER:
      break;
  }
}

void write_instruction(assembler_t *assembler, instruction_t *instruction) {
  machine_word_t first_word = make_code_word(instruction->info->opcode,
                                             instruction->num_args >= 1 ? instruction->operand_1.kind : 0,
                                             instruction->num_args >= 2 ? instruction->operand_2.kind : 0,
                                             0 /*TODO*/);
  add_code_word(assembler, first_word);
}

bool_t compile_statement(assembler_t *assembler, statement_t *statement) {
  if (statement->kind == STATEMENT_INSTRUCTION) {
    instruction_t *instruction = &statement->data.instruction;
    if (instruction->num_args != instruction->info->arg_amount) {
      /* Instruction ___ expected _ args but got _ */
      return FALSE;
    }
  }
  if (statement->has_label) {
    if (statement->kind == STATEMENT_INSTRUCTION) {
    }
  }
}

void write_statement(assembler_t *assembler, statement_t *statement) {
}

/* TODO temp arguments */
bool_t first_pass(char *input_file_path, char *output_file_path, assembler_t *assembler) {
  FILE *in, *out;
  char line[MAX_LINE];
  bool_t error_flag = FALSE;
  int line_number = 1, total_errors = 0;

  in = fopen(input_file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    printf("Couldn't open input file\n");
    return FALSE;
  }

  while (read_line(in, line) != SENTENCE_EOF) {
    statement_t statement;
    result_t res = parse_statement(line, &statement);

    if (res != SUCCESS) {
      printf("Error at %s:%d: %s\n", input_file_path, line_number, res);
      error_flag = TRUE;
      total_errors++;
    }
    else if (statement.kind != STATEMENT_EMPTY) {
      compile_statement(assembler, &statement);
    }

    line_number++;
  }

  merge_data(assembler);
  /*TODO: Merge between DC and IC into label_table.*/

  // merge_dc_to_ic()
  printf("Total Errors: %d\n", total_errors);
  return error_flag;
}
