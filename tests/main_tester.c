
#include "lib/unity.h"
#include "lib/unity_internals.h"
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/linked_list.h"
#include "../firstpass/parser.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_one_arg_no_args(void){
    char *s = "                             ";
    args_t arg = ONE_ARG;
    int result = parse_instruction_args(&s, arg);

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "should return false, expected arguement got EOF");
}

void test_one_arg_number_decimal(void){
  char *s = "#-5.12";
  args_t arg = ONE_ARG;
  int result = parse_instruction_args(&s, arg);

 TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Assembler only accepts whole numbers");
}

void test_one_arg_number_whole(void){
  char *s = "#-105";
  args_t arg = ONE_ARG;

  int result = parse_instruction_args(&s, arg);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Assembler only accepts numbers");
}

void test_one_arg_number_whole_2(void){
    char *s = "               #105                    ";
    args_t arg = ONE_ARG;

    int result = parse_instruction_args(&s, arg);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Assembler only accepts numbers");
}

void test_one_arg_number_whole_3(void){
    char *s = "               #           -55                    ";
    args_t arg = ONE_ARG;

    int result = parse_instruction_args(&s, arg);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Shouldn't parse spaces between one arg.");
}


/*R1 => R8*/








// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_one_arg_no_args);
    RUN_TEST(test_one_arg_number_decimal);
    RUN_TEST(test_one_arg_number_whole);
    RUN_TEST(test_one_arg_number_whole_2);
    RUN_TEST(test_one_arg_number_whole_3);

    return UNITY_END();
}