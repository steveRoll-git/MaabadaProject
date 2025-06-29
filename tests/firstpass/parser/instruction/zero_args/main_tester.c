#include "../../../../lib/unity.h"
#include "../../../../lib/unity_internals.h"
#include "../../../../../common/data.h"
#include "../../../../../common/utils.h"
#include "../../../../../datatypes/linked_list.h"
#include "../../../../../firstpass/parser.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


void test_parse_instruction_zero_args1(void) {
 char *s = "                           ";
 int result = parse_instruction_args(&s, 0);
 TEST_ASSERT_EQUAL_INT_MESSAGE(1, result, "Empty Space counts as no arguement");
}

void test_parse_instruction_zero_args2(void) {
    char *s = "       R1 ";
    int result = parse_instruction_args(&s, 0);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Zero arguements instruction parsing MUST only have whitespace as args.");
}



// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
   RUN_TEST(test_parse_instruction_zero_args1);
   RUN_TEST(test_parse_instruction_zero_args2);
    return UNITY_END();
}