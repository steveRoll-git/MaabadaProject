#include "../include/codegen.h"

#include <stdio.h>

#include "../include/assembler.h"
#include "../include/data.h"
#include "../include/parser.h"

#define OPCODE_FIRST_BIT 6
#define OPCODE_MASK 0xf
#define OPCODE_BITS(n) ((n & OPCODE_MASK) << OPCODE_FIRST_BIT)

#define SRC_FIRST_BIT 4
#define SRC_MASK 0x3
#define SRC_BITS(n) ((n & SRC_MASK) << SRC_FIRST_BIT)

#define DST_FIRST_BIT 2
#define DST_MASK 0x3
#define DST_BITS(n) ((n & DST_MASK) << DST_FIRST_BIT)

#define REG1_FIRST_BIT 6
#define REG2_FIRST_BIT 2
#define REG_MASK 0xf
#define REG1_BITS(n) ((n & REG_MASK) << REG1_FIRST_BIT)
#define REG2_BITS(n) ((n & REG_MASK) << REG2_FIRST_BIT)

#define IMM_FIRST_BIT 2
#define IMM_MASK 0xff
#define IMM_BITS(n) ((n & IMM_MASK) << IMM_FIRST_BIT)

#define DATA_MASK 0x3ff

machine_word_t make_code_word(unsigned int opcode, unsigned int src_operand, unsigned int dst_operand, int era) {
  machine_word_t word = 0;
  word |= OPCODE_BITS(opcode);
  word |= SRC_BITS(src_operand);
  word |= DST_BITS(dst_operand);
  return word;
}

/* Creates a word where one register occupies bits 6-9 and the other occupies bits 2-5. */
machine_word_t make_joined_register_word(char reg_1, char reg_2) {
  return REG1_BITS(reg_1) | REG2_BITS(reg_2);
}

void write_operand(assembler_t *assembler, int line_number, operand_t *operand, bool_t is_second) {
  switch (operand->kind) {
    case OPERAND_KIND_IMMEDIATE:
      add_code_word(assembler, IMM_BITS(operand->data.immediate));
      break;

    case OPERAND_KIND_LABEL:
      add_label_reference(assembler, operand->data.label, line_number);
      break;

    case OPERAND_KIND_MATRIX:
      /* The first word in matrix addressing is the address of the label. */
      add_label_reference(assembler, operand->data.matrix.label, line_number);
      add_code_word(assembler, make_joined_register_word(operand->data.matrix.row_reg, operand->data.matrix.col_reg));
      break;

    case OPERAND_KIND_REGISTER:
      if (is_second) {
        add_code_word(assembler, REG2_BITS(operand->data.register_index));
      }
      else {
        add_code_word(assembler, REG1_BITS(operand->data.register_index));
      }
      break;
  }
}

void write_instruction(assembler_t *assembler, int line_number, instruction_t *instruction) {
  operand_t *operand_1 = &instruction->operand_1;
  operand_t *operand_2 = &instruction->operand_2;

  add_code_word(assembler,
                make_code_word(instruction->info->opcode,
                               instruction->num_args >= 1 ? operand_1->kind : 0,
                               instruction->num_args >= 2 ? operand_2->kind : 0,
                               ENCODING_ABSOLUTE /*TODO?*/));

  if (operand_1->kind == OPERAND_KIND_REGISTER && operand_2->kind == OPERAND_KIND_REGISTER) {
    /* If both operands are registers, we write a single word that contains both of them. */
    add_code_word(assembler, make_joined_register_word(operand_1->data.register_index, operand_2->data.register_index));
  }
  else {
    if (instruction->num_args >= ONE_ARG) {
      write_operand(assembler, line_number, &instruction->operand_1, FALSE);
    }
    if (instruction->num_args == TWO_ARGS) {
      write_operand(assembler, line_number, &instruction->operand_2, TRUE);
    }
  }
}

void write_directive(assembler_t *assembler, directive_t *directive) {
  int i;
  switch (directive->kind) {
    case DIRECTIVE_KIND_DATA:
    case DIRECTIVE_KIND_STRING:
    case DIRECTIVE_KIND_MAT:
      for (i = 0; i < directive->info.data.size; i++) {
        add_data_word(assembler, directive->info.data.array[i]);
      }
      break;
    case DIRECTIVE_KIND_ENTRY:
      /*TODO*/
      break;
    case DIRECTIVE_KIND_EXTERN:
      /*TODO*/
      break;
    default:
      break;
  }
}

result_t compile_statement(assembler_t *assembler, int line_number, statement_t *statement) {
  if (statement->kind == STATEMENT_EMPTY) {
    return SUCCESS;
  }

  if (statement->has_label) {
    TRY(add_label(assembler, statement->label, statement->kind == STATEMENT_DIRECTIVE, FALSE))
  }

  if (statement->kind == STATEMENT_INSTRUCTION) {
    instruction_t *instruction = &statement->data.instruction;
    instruction_info_t *info = instruction->info;

    if (instruction->num_args != info->arg_amount) {
      /* The number of arguments given does not match the number of arguments the instruction expects. */
      return instruction->num_args > info->arg_amount ? ERR_TOO_MANY_ARGS : ERR_NOT_ENOUGH_ARGS;
    }

    if (instruction->num_args >= ONE_ARG) {
      /* Make sure that the instruction is called with operand kinds that it supports. */
      /* All instructions support label and matrix addressing, so only immediate and register operands are checked. */

      operand_t *src = &instruction->operand_1, *dst = &instruction->operand_1;

      if (instruction->num_args == TWO_ARGS) {
        /* If an instruction has two arguments, the first one is the source and the second is the destination. */
        dst = &instruction->operand_2;
        if (!info->src_immediate_register &&
            (src->kind == OPERAND_KIND_IMMEDIATE || src->kind == OPERAND_KIND_REGISTER)) {
          return src->kind == OPERAND_KIND_IMMEDIATE ? ERR_SRC_IMMEDIATE : ERR_SRC_REGISTER;
        }
      }

      if (!info->dst_immediate && dst->kind == OPERAND_KIND_IMMEDIATE) {
        return ERR_DST_IMMEDIATE;
      }
    }

    write_instruction(assembler, line_number, instruction);
  }
  else if (statement->kind == STATEMENT_DIRECTIVE) {
    write_directive(assembler, &statement->data.directive);
  }

  return SUCCESS;
}

bool_t codegen(char *input_file_path, assembler_t *assembler) {
  FILE *in, *out;
  char line[MAX_LINE];
  bool_t success = TRUE;
  int line_number = 1, total_errors = 0;

  in = fopen(input_file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    printf("Couldn't open input file\n");
    return FALSE;
  }

  while (read_line(in, line) != SENTENCE_EOF) {
    statement_t statement;
    result_t result = parse_statement(line, &statement);

    if (result == SUCCESS) {
      result = compile_statement(assembler, line_number, &statement);
    }

    if (result != SUCCESS) {
      print_error(input_file_path, line_number, result);
      success = FALSE;
      total_errors++;
    }

    line_number++;
  }

  printf("Total Errors: %d\n", total_errors);
  return success;
}
