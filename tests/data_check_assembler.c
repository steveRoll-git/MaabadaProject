#include <stdio.h>
#include <stdlib.h>
#include "../include/data.h"
#include "../include/parser.h"
#include "common.h"

static instruction_t instruction;

int main() {
  char *s = "                             ";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == NO_ARGS)

  s = "#-5.12";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS);

  s = "#-105";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)
  ASSERT(instruction.operand_1.kind == OPERAND_KIND_IMMEDIATE)

  s = "#+105";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)
  ASSERT(instruction.operand_1.kind == OPERAND_KIND_IMMEDIATE)

  s = "#+105x";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS)

  s = "               #105                    ";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)
  ASSERT(instruction.operand_1.kind == OPERAND_KIND_IMMEDIATE)

  s = "               #           -55                    ";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS)

  s = "               r0                   ";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)
  ASSERT(instruction.operand_1.kind == OPERAND_KIND_REGISTER)

  s = "       r       0         ";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS)

  s = "r";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)
  ASSERT(instruction.operand_1.kind == OPERAND_KIND_LABEL)

  s = "stop";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS)

  return 0;
}
