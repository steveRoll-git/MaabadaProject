#include <stdio.h>
#include <stdlib.h>
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "../firstpass/parser.h"

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

  s = "               #105                    ";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)

  s = "               #           -55                    ";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS)

  s = "               r0                   ";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)

  s = "       r       0         ";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS)

  s = "r";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)

  s = "stop";
  ASSERT(parse_instruction_operands(s, &instruction) != SUCCESS)

  return 0;
}
