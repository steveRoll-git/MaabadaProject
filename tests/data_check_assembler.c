#include <stdio.h>
#include <stdlib.h>
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "../firstpass/parser.h"

#include "common.h"

int main() {
  assembler_t assembler = assembler_create();
  char *s = "                             ";
  args_t arg = ONE_ARG;
  int result = parse_instruction_args(&s, arg, &assembler);
  ASSERT(result == 0)
  //  TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "should return false, expected arguement got EOF");


  s = "#-5.12";
  result = parse_instruction_args(&s, arg, &assembler);
  ASSERT(result == 0)

  // //  TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Assembler only accepts whole numbers");
  ASSERT(assembler.ic == 103)
  // TEST_ASSERT_EQUAL_INT(102, assembler.ic);
  s = "#-105";
  result = parse_instruction_args(&s, arg, &assembler);
  ASSERT(result == 1)
  // //   TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Assembler only accepts numbers");
  ASSERT(105 == assembler.ic);

  //
  s = "               #105                    ";
  result = parse_instruction_args(&s, arg, &assembler);
  // TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Assembler only accepts numbers");
  // TEST_ASSERT_EQUAL_INT(102, assembler.ic);
  ASSERT(result == 1)
  ASSERT(assembler.ic == 107)
  s = "               #           -55                    ";
  result = parse_instruction_args(&s, arg, &assembler);
  ASSERT(result == 0);
  // // TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Shouldn't parse spaces between one arg.");
  //
  //
  s = "               r0                   ";
  result = parse_instruction_args(&s, arg, &assembler);
  ASSERT(result == 1)
  // // TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Valid Register should be accepted.");
  // TEST_ASSERT_EQUAL_INT(102, assembler.ic);

  //
  // // s = (char *) malloc(sizeof(char) * 3);
  // // for (i = 0; i <= 7; i++) {
  // //   s[0] = 'r';
  // //   s[1] = '0' + i;
  // //   s[2] = '\0';
  // //   int result = parse_instruction_args(&s, arg, &assembler);
  // //   TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Valid Register should be accepted.");
  // // }
  //
  //
  s = "       r       0         ";
  result = parse_instruction_args(&s, arg, &assembler);
  ASSERT(result == 0)
  // // TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Register name with space doesn't count.");
  //
  //
  s = "r";
  result = parse_instruction_args(&s, arg, &assembler);
  ASSERT(result == 1);
  // // TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "All Valid Labels are welcomed.");
  // // TEST_ASSERT_EQUAL_INT(102, assembler.ic);
  //
  //
  s = "stop";
  result = parse_instruction_args(&s, arg, &assembler);

  // // TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Arguements Can't be special commands");
  ASSERT(result == 0);

  return 0;
}
