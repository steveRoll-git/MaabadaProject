#include <string.h>

#include "common.h"

static statement_t statement;

int main() {
  ASSERT(parse_statement("label: add #6, r2", &statement) == SUCCESS)
  ASSERT(statement.kind == STATEMENT_INSTRUCTION)
  ASSERT(statement.has_label)
  ASSERT(strcmp(statement.label, "label") == 0)
  ASSERT(statement.data.instruction.info->opcode == OPCODE_ADD)
  ASSERT(statement.data.instruction.num_args == TWO_ARGS)
  ASSERT(statement.data.instruction.operand_1.kind == OPERAND_KIND_IMMEDIATE)
  ASSERT(statement.data.instruction.operand_1.data.immediate == 6)
  ASSERT(statement.data.instruction.operand_2.kind == OPERAND_KIND_REGISTER)
  ASSERT(statement.data.instruction.operand_2.data.register_index == 2)

  ASSERT(parse_statement(";some comment", &statement) == SUCCESS)
  ASSERT(statement.kind == STATEMENT_EMPTY)

  ASSERT(parse_statement("     \t\t   ", &statement) == SUCCESS)
  ASSERT(statement.kind == STATEMENT_EMPTY)

  ASSERT(parse_statement("mov M1[ r3 ][ r6 ], YYY", &statement) == SUCCESS)
  ASSERT(statement.kind == STATEMENT_INSTRUCTION)
  ASSERT(statement.data.instruction.info->opcode == OPCODE_MOV)
  ASSERT(statement.data.instruction.num_args == TWO_ARGS)
  ASSERT(statement.data.instruction.operand_1.kind == OPERAND_KIND_MATRIX)
  ASSERT(statement.data.instruction.operand_1.data.matrix.row_reg == 3)
  ASSERT(statement.data.instruction.operand_1.data.matrix.col_reg == 6)
  ASSERT(strcmp(statement.data.instruction.operand_1.data.matrix.label, "M1") == 0)
  ASSERT(statement.data.instruction.operand_2.kind == OPERAND_KIND_LABEL)
  ASSERT(strcmp(statement.data.instruction.operand_2.data.label, "YYY") == 0)

  ASSERT(parse_statement("lab_el: add #6, r2", &statement) != SUCCESS)
  ASSERT(parse_statement("label : mov r1, r2", &statement) != SUCCESS)
  ASSERT(parse_statement("label:mov r1, r2", &statement) != SUCCESS)
  ASSERT(parse_statement("       ;some comment", &statement) != SUCCESS)
  ASSERT(parse_statement("mov M1 [r3][r6], YYY", &statement) != SUCCESS)
  ASSERT(parse_statement("mov M1[r3] [r6], YYY", &statement) != SUCCESS)

  return EXIT_SUCCESS;
}
