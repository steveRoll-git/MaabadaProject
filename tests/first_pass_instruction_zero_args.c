#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "../firstpass/parser.h"
#include "common.h"

int test_parse_instruction_zero_args1(void) {
  assembler_t assembler = assembler_create();
  char *s = "             A              ";
  int result = parse_instruction_args(&s, NO_ARGS, &assembler);
  ASSERT(0 == result)
}

int test_parse_instruction_zero_args2(void) {
  assembler_t assembler = assembler_create();
  char *s = "       R1 ";
  int result = parse_instruction_args(&s, NO_ARGS, &assembler);
  ASSERTM(result == 0, "Zero arguements instruction parsing MUST only have whitespace as args.");

  return 0;
}


// not needed when using generate_test_runner.rb
int main(void) {
  test_parse_instruction_zero_args1();
  test_parse_instruction_zero_args2();
  return 0;
}
