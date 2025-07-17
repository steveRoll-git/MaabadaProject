#include "../include/codegen.h"

#include <stdio.h>

#include "../include/context.h"
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

#define ERA_MASK 0x3
#define ERA_BITS(n) (n & ERA_MASK)

#define REG1_FIRST_BIT 6
#define REG2_FIRST_BIT 2
#define REG_MASK 0xf
#define REG1_BITS(n) ((n & REG_MASK) << REG1_FIRST_BIT)
#define REG2_BITS(n) ((n & REG_MASK) << REG2_FIRST_BIT)

#define IMM_FIRST_BIT 2
#define IMM_MASK 0xff
#define IMM_BITS(n) ((n & IMM_MASK) << IMM_FIRST_BIT)

#define DATA_MASK 0x3ff

/* Returns the first word of an instruction based on the opcode and the source/destination operands. */
machine_word_t make_code_word(instruction_t *instruction) {
  machine_word_t word = 0;

  word |= OPCODE_BITS(instruction->info->opcode);

  if (instruction->num_args == ONE_ARG) {
    /* If the instruction has just one operand, it is the destination operand. */
    word |= DST_BITS(instruction->operand_1.kind);
  }
  else if (instruction->num_args == TWO_ARGS) {
    /* If the instruction has two operands, the first is the source, and the second is the destination. */
    word |= SRC_BITS(instruction->operand_1.kind);
    word |= DST_BITS(instruction->operand_2.kind);
  }

  return word;
}

/* Creates a word where one register occupies bits 6-9 and the other occupies bits 2-5. */
machine_word_t make_joined_register_word(char reg_1, char reg_2) {
  return REG1_BITS(reg_1) | REG2_BITS(reg_2);
}

/* Outputs the operand's binary code into the assembler's code image. */
/* If the operand references a label, its location will be added to the label table, and resolved later. */
/* May fail if memory allocations did not succeed. */
result_t write_operand(context_t *context, operand_t *operand, bool_t is_second) {
  switch (operand->kind) {
    case OPERAND_KIND_IMMEDIATE:
      TRY(add_code_word(context, IMM_BITS(operand->data.immediate)))
      break;

    case OPERAND_KIND_LABEL:
      TRY(add_label_reference(context, operand->data.label))
      break;

    case OPERAND_KIND_MATRIX:
      /* The first word in matrix addressing is the address of the label. */
      TRY(add_label_reference(context, operand->data.matrix.label))
      TRY(add_code_word(context, make_joined_register_word(operand->data.matrix.row_reg, operand->data.matrix.col_reg)))
      break;

    case OPERAND_KIND_REGISTER:
      if (is_second) {
        TRY(add_code_word(context, REG2_BITS(operand->data.register_index)))
      }
      else {
        TRY(add_code_word(context, REG1_BITS(operand->data.register_index)))
      }
      break;
  }

  return SUCCESS;
}

/* Outputs the instruction's binary code into the assembler's code image. */
/* May fail if memory allocations did not succeed. */
result_t write_instruction(context_t *context, instruction_t *instruction) {
  operand_t *operand_1 = &instruction->operand_1;
  operand_t *operand_2 = &instruction->operand_2;

  TRY(add_code_word(context, make_code_word(instruction)))

  if (operand_1->kind == OPERAND_KIND_REGISTER && operand_2->kind == OPERAND_KIND_REGISTER) {
    /* If both operands are registers, we write a single word that contains both of them. */
    machine_word_t word = make_joined_register_word(operand_1->data.register_index, operand_2->data.register_index);
    TRY(add_code_word(context, word))
  }
  else {
    /* Write the operands one after another. */
    if (instruction->num_args >= ONE_ARG) {
      TRY(write_operand(context, &instruction->operand_1, FALSE))
    }
    if (instruction->num_args == TWO_ARGS) {
      TRY(write_operand(context, &instruction->operand_2, TRUE))
    }
  }

  return SUCCESS;
}

/* Outputs the directive's binary code into the assembler's data image. */
/* May fail if memory allocations did not succeed. */
result_t write_directive(context_t *context, directive_t *directive) {
  int i;
  switch (directive->kind) {
    case DIRECTIVE_KIND_DATA:
    case DIRECTIVE_KIND_STRING:
    case DIRECTIVE_KIND_MAT:
      for (i = 0; i < directive->info.data.size; i++) {
        TRY(add_data_word(context, directive->info.data.array[i]))
      }
      break;

    case DIRECTIVE_KIND_ENTRY:
      TRY(add_entry(context, directive->info.label))
      break;

    case DIRECTIVE_KIND_EXTERN:
      TRY(add_extern(context, directive->info.label))
      break;

    default:
      break;
  }

  return SUCCESS;
}

/* Checks that a statement is well-formed, and generates its code. */
/* May fail if: */
/* - The statement is semantically incorrect. (For example, if an instruction is called with unsupported operands.) */
/* - The statement defines a label that was already defined. */
/* - Memory allocations did not succeed. */
result_t compile_statement(context_t *context, statement_t *statement) {
  if (statement->kind == STATEMENT_EMPTY) {
    /* Empty statements require no action. */
    return SUCCESS;
  }

  if (statement->has_label) {
    /* If the statement has a label, add it to the label table. */
    TRY(add_label(context, statement->label, statement->kind == STATEMENT_DIRECTIVE))
  }

  if (statement->kind == STATEMENT_INSTRUCTION) {
    /* Before generating code for the instruction, check that it's called correctly. */
    /* (With the correct number of operands, and the correct kinds of operands.) */
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

    write_instruction(context, instruction);
  }
  else if (statement->kind == STATEMENT_DIRECTIVE) {
    TRY(write_directive(context, &statement->data.directive))
  }

  return SUCCESS;
}

result_t codegen(context_t *context) {
  FILE *in;
  char line[MAX_LINE];
  bool_t success = TRUE;
  result_t codegen_result = SUCCESS;
  int total_errors = 0;

  in = fopen(context->file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    return ERR_INPUT_FILE_FAIL;
  }

  context->line_number = 1;

  while (read_line(in, line) != READ_LINE_EOF) {
    statement_t statement;
    result_t result = parse_statement(line, &statement);

    if (result == SUCCESS) {
      /* If the line was successfully parsed, compile it. */
      result = compile_statement(context, &statement);
    }

    if (result == ERR_OUT_OF_MEMORY) {
      /* If we ran out of memory, data could be in an invalid state (because memory was not allocated), */
      /* so we stop processing any more lines. */
      codegen_result = ERR_OUT_OF_MEMORY;
      break;
    }

    if (result != SUCCESS) {
      /* If the line has incorrect syntax or couldn't be compiled, print an error. */
      print_error(context->file_path, context->line_number, result);
      codegen_result = ERR_CODEGEN_FAILED;
      total_errors++;
    }

    context->line_number++;
  }

  fclose(in);

  if (total_errors > 0) {
    printf("Total Errors: %d\n", total_errors);
  }

  return codegen_result;
}
