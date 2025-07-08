#include "../common/data.h"
#include "../firstpass/parser.h"
#include "common.h"

static instruction_t instruction;

int test_parse_instruction_zero_args1(void) {
  char *s = "             A              ";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)
  return TRUE;
}

int test_parse_instruction_zero_args2(void) {
  char *s = "       R1 ";
  ASSERT(parse_instruction_operands(s, &instruction) == SUCCESS)
  ASSERT(instruction.num_args == ONE_ARG)

  return TRUE;
}


// not needed when using generate_test_runner.rb
int main(void) {
  test_parse_instruction_zero_args1();
  test_parse_instruction_zero_args2();
  return 0;
}
