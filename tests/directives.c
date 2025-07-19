#include <string.h>

#include "common.h"

#define ARRAY_SIZE(arr, type) (sizeof(arr) / sizeof(type))

#define ARR(...) __VA_ARGS__

static statement_t statement;

int check_data_equals(const machine_word_t *expected_data, int size) {
  int i;
  if (statement.data.directive.info.data.size != size) {
    return FALSE;
  }
  for (i = 0; i < size; i++) {
    if (statement.data.directive.info.data.array[i] != expected_data[i]) {
      return FALSE;
    }
  }
  return TRUE;
}

#define ASSERT_DATA_EQUALS(expected_arr)                                                                               \
  {                                                                                                                    \
    machine_word_t expected_data[] = expected_arr;                                                                     \
    ASSERT(check_data_equals(expected_data, ARRAY_SIZE(expected_data, machine_word_t)))                                \
  }

int main(void) {
  ASSERT(parse_statement(".data 1, 2, 9, 4", &statement) == SUCCESS)
  ASSERT_DATA_EQUALS(ARR({1, 2, 9, 4}))

  ASSERT(parse_statement("somelabel: .mat[2][3] 5, 2, 2  , 6, 23, 5    ", &statement) == SUCCESS)
  ASSERT_DATA_EQUALS(ARR({5, 2, 2, 6, 23, 5}))

  ASSERT(parse_statement("   .mat   [ 1   ] [ 1 ] -65", &statement) == SUCCESS)
  ASSERT_DATA_EQUALS(ARR({-65}))

  ASSERT(parse_statement("somelabel: .mat[2][3] 4, 3, 2", &statement) == SUCCESS)
  ASSERT_DATA_EQUALS(ARR({4, 3, 2, 0, 0, 0}))

  ASSERT(parse_statement(".string \"abc\"xyz\"", &statement) == SUCCESS)
  ASSERT_DATA_EQUALS(ARR({'a', 'b', 'c', '"', 'x', 'y', 'z', '\0'}))

  ASSERT(parse_statement(".entry someLabel", &statement) == SUCCESS)
  ASSERT(statement.data.directive.kind == DIRECTIVE_KIND_ENTRY)
  ASSERT(strcmp(statement.data.directive.info.label, "someLabel") == 0)

  ASSERT(parse_statement(".extern someLabel", &statement) == SUCCESS)
  ASSERT(statement.data.directive.kind == DIRECTIVE_KIND_EXTERN)
  ASSERT(strcmp(statement.data.directive.info.label, "someLabel") == 0)

  ASSERT(parse_statement(".data   a", &statement) != SUCCESS)
  ASSERT(parse_statement(".data   ", &statement) != SUCCESS)
  ASSERT(parse_statement(".data   1, 2, 3 asdf", &statement) != SUCCESS)
  ASSERT(parse_statement(".string \"abc", &statement) != SUCCESS)
  ASSERT(parse_statement(".string \"abc\" extra text", &statement) != SUCCESS)
  ASSERT(parse_statement("   .mat   [ 0   ] [ 1 ] -65", &statement) != SUCCESS)
  ASSERT(parse_statement("   .mat   [ 1   ] [ 1 ] 123 vjskdfo", &statement) != SUCCESS)
  ASSERT(parse_statement(".entry ", &statement) != SUCCESS)
  ASSERT(parse_statement(".entry someLabel asdf", &statement) != SUCCESS)
  ASSERT(parse_statement(".extern someLabel asdf", &statement) != SUCCESS)
  ASSERT(parse_statement(".asdf", &statement) != SUCCESS)
  ASSERT(parse_statement(".dataa 1, 3", &statement) != SUCCESS)
  ASSERT(parse_statement(".stringa \"a\"", &statement) != SUCCESS)
  ASSERT(parse_statement(". 1, 3", &statement) != SUCCESS)

  return 0;
}
