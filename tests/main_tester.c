#include <stdlib.h>
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/linked_list.h"
#include "../firstpass/parser.h"
#include "lib/unity.h"
#include "lib/unity_internals.h"
#
assembler_t assembler;
void setUp(void) {
  // set stuff up here
  assembler = assembler_create();
}

void tearDown(void) {
  // clean stuff up here
}

void test_one_arg_no_args(void) {
  char *s = "                             ";
  args_t arg = ONE_ARG;
  int result = parse_instruction_args(&s, arg, &assembler);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "should return false, expected arguement got EOF");
}

void test_one_arg_number_decimal(void) {
  char *s = "#-5.12";
  args_t arg = ONE_ARG;
  int result = parse_instruction_args(&s, arg, &assembler);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Assembler only accepts whole numbers");
}

void test_one_arg_number_whole(void) {
  char *s = "#-105";
  args_t arg = ONE_ARG;

  int result = parse_instruction_args(&s, arg, &assembler);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Assembler only accepts numbers");
}

void test_one_arg_number_whole_2(void) {
  char *s = "               #105                    ";
  args_t arg = ONE_ARG;

  int result = parse_instruction_args(&s, arg, &assembler);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Assembler only accepts numbers");
}

void test_one_arg_number_whole_3(void) {
  char *s = "               #           -55                    ";
  args_t arg = ONE_ARG;

  int result = parse_instruction_args(&s, arg, &assembler);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Shouldn't parse spaces between one arg.");
}


void test_one_arg_valid_register_R0(void) {
  char *s = "               R0                   ";
  args_t arg = ONE_ARG;

  int result = parse_instruction_args(&s, arg, &assembler);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Valid Register should be accepted.");
}


void test_one_arg_valid_register_ALL_REGISTERS(void) {
  int i;

  args_t arg = ONE_ARG;
  char *s = (char *) malloc(sizeof(char) * 3);
  for (i = 0; i <= 7; i++) {
    s[0] = 'R';
    s[2] = '\0';
    s[1] = '0' + i;
    int result = parse_instruction_args(&s, arg, &assembler);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Valid Register should be accepted.");
  }
}

void test_one_arg_valid_register_space(void) {
  int i;
  args_t arg = ONE_ARG;
  char *s = "       R       0         ";

  int result = parse_instruction_args(&s, arg, &assembler);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Register name with space doesn't count.");
}

void test_one_arg_valid_register_special_letter(void) {
  int i;

  args_t arg = ONE_ARG;
  char *s = "R";

  int result = parse_instruction_args(&s, arg, &assembler);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "All Valid Labels are welcomed.");
}

void test_one_arg_is_arg_a_command(void) {
  int i;

  args_t arg = ONE_ARG;
  char *s = "mov";

  int result = parse_instruction_args(&s, arg, &assembler);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Arguements Can't be special commands");
}


// not needed when using generate_test_runner.rb
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_one_arg_no_args);
  RUN_TEST(test_one_arg_number_decimal);
  RUN_TEST(test_one_arg_number_whole);
  RUN_TEST(test_one_arg_number_whole_2);
  RUN_TEST(test_one_arg_number_whole_3);
  RUN_TEST(test_one_arg_valid_register_R0);
  RUN_TEST(test_one_arg_valid_register_ALL_REGISTERS);
  RUN_TEST(test_one_arg_valid_register_space);
  RUN_TEST(test_one_arg_valid_register_special_letter);
  RUN_TEST(test_one_arg_is_arg_a_command);

  return UNITY_END();
}
