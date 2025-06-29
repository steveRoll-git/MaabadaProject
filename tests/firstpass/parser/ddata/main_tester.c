#include "../../../lib/unity.h"
#include "../../../lib/unity_internals.h"
#include "../../../../common/data.h"
#include "../../../../common/utils.h"
#include "../../../../datatypes/linked_list.h"
#include "../../../../firstpass/parser.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_parse_ddata_work_example1(void) {
    //more test stuff
    int i = 0;
    char *token = "5  , 1 ,    -51,    29   ,18   , 2";
    linked_list_t macro_table = list_init();
    int val = parse_ddata(&token, &macro_table);

    int* items = list_items(&macro_table);
    int expected[] = {5, 1, -51, 29, 18 ,2};

   	for(i = 0; i < 6;i++) {
          printf("%d\t", items[i]);
   	}
        printf("\n");

    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected, items, (6), "");

    TEST_ASSERT_EQUAL_INT(1, val);
}

void test_parse_ddata_work_example2(void) {
    int i = 0;
    char *token = "1 , 2, 3,, 4";
    linked_list_t macro_table = list_init();
    int val = parse_ddata(&token, &macro_table);

    int* items = list_items(&macro_table);
    int expected[] = {1,2,3,4};
    TEST_ASSERT_EQUAL_INT(0, val);
}


void test_parse_ddata_work_example3(void) {
    int i = 0;
    char *token = "1 , 2, 3,- 4";
    linked_list_t macro_table = list_init();
    int val = parse_ddata(&token, &macro_table);

    int* items = list_items(&macro_table);
    int expected[] = {1,2,3,4};
    TEST_ASSERT_EQUAL_INT(0, val);
}
void test_parse_ddata_last_comma(void) {
    int i = 0;
    char *token = "1 , 2, 3 , ";
    linked_list_t macro_table = list_init();
    int val = parse_ddata(&token, &macro_table);

    int* items = list_items(&macro_table);
    TEST_ASSERT_EQUAL_INT(0, val);
}

void test_parse_ddata_empty_string(void){
char *token = "";
linked_list_t macro_table = list_init();
int val = parse_ddata(&token, &macro_table);
TEST_ASSERT_EQUAL_INT(0, val);
}


// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parse_ddata_work_example1);
    RUN_TEST(test_parse_ddata_work_example2);
    RUN_TEST(test_parse_ddata_work_example3);
    RUN_TEST(test_parse_ddata_empty_string);
    RUN_TEST(test_parse_ddata_last_comma);
    return UNITY_END();
}