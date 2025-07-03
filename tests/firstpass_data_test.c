#include <stdlib.h>

#include "common.h"

#include "../common/utils.h"
#include "../firstpass/parser.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_parse_ddata_work_example1(void) {
  // more test stuff
  int i = 0;
  char *token = "5  , 1 ,    -51,    29   ,18   , 2";
  assembler_t assembler = assembler_create();
  int val = parse_data(token, &assembler);

  int *items = assembler.data_array.ptr;
  int expected[] = {5, 1, -51, 29, 18, 2};

  for (i = 0; i < 6; i++) {
    printf("%d\t", items[i]);
  }
  printf("\n");

  for (i = 0; i < 6; i++) {
    ASSERT(expected[i] == items[i]);
  }

  ASSERT(val == 1)
}

void test_parse_ddata_work_example2(void) {
  int i = 0;
  char *token = "1 , 2, 3,, 4";
  assembler_t assembler = assembler_create();
  int val = parse_data(token, &assembler);

  // TEST_ASSERT_EQUAL_INT(0, val);
  ASSERT(0 == val);
}


void test_parse_ddata_work_example3(void) {
  int i = 0;
  char *token = "1 , 2, 3,- 4";
  assembler_t assembler = assembler_create();
  int val = parse_data(token, &assembler);

  // TEST_ASSERT_EQUAL_INT(0, val);
  ASSERT(0 == val);
}

void test_parse_ddata_last_comma(void) {
  char *token = "1 , 2, 3 , ";
  assembler_t assembler = assembler_create();
  int val = parse_data(token, &assembler);

  // TEST_ASSERT_EQUAL_INT(0, val);
  ASSERT(0 == val);
}


void test_parse_ddata_empty_string(void) {
  char *token = "";
  assembler_t assembler = assembler_create();
  int val = parse_data(token, &assembler);
  // TEST_ASSERT_EQUAL_INT(0, val);
  ASSERT(0 == val);
}


// not needed when using generate_test_runner.rb
int main(void) {

  test_parse_ddata_work_example1();
  test_parse_ddata_work_example2();
  test_parse_ddata_work_example3();
  test_parse_ddata_empty_string();
  test_parse_ddata_last_comma();
  return 0;
}
