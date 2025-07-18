#include <stdlib.h>

#include "common.h"

#include "../headers/parser.h"

static directive_t directive;

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
  ASSERT(parse_data(token, &directive) == SUCCESS)

  machine_word_t *items = directive.info.data.array;
  machine_word_t expected[] = {5, 1, -51, 29, 18, 2};

  for (i = 0; i < 6; i++) {
    ASSERT(expected[i] == items[i]);
  }
}

void test_parse_ddata_work_example2(void) {
  int i = 0;
  char *token = "1 , 2, 3,, 4";
  ASSERT(parse_data(token, &directive) != SUCCESS)
}


void test_parse_ddata_work_example3(void) {
  char *token = "1 , 2, 3,- 4";
  ASSERT(parse_data(token, &directive) != SUCCESS)
}

void test_parse_ddata_last_comma(void) {
  char *token = "1 , 2, 3 , ";
  ASSERT(parse_data(token, &directive) != SUCCESS)
}


void test_parse_ddata_empty_string(void) {
  char *token = "";
  ASSERT(parse_data(token, &directive) != SUCCESS)
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
