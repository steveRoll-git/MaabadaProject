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
  ASSERT(statement.data.instruction.operand_2.kind == OPERAND_KIND_REGISTER)

  ASSERT(parse_statement("label : mov r1, r2", &statement) != SUCCESS)

  return EXIT_SUCCESS;
}
