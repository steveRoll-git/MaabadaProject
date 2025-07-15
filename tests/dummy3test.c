#include <string.h>

#include "common.h"

#define ASSERT_LABEL(label, expected)                                                                                  \
  ASSERT(((label_info_t *) table_get(assembler->label_table, label))->value == expected)

int main(void) {
  char *in = "../examples/wont_work/matricesTesting.am";
  table_t *macro_table;
  table_create(0, &macro_table);
  assembler_t *assembler;
  assembler_create(in, macro_table, &assembler);
  bool_t success = codegen(assembler);
  ASSERT(success);
  merge_data(assembler);
  print_data(assembler);

  ASSERT_LABEL("MAIN", 100)

  ASSERT_LABEL("LOOP", 107)
  ASSERT_LABEL("END", 121)
  ASSERT_LABEL("STR", 122)
  ASSERT_LABEL("LENGTH", 129)
  ASSERT_LABEL("K", 132)
  ASSERT_LABEL("M1", 133)

  int expected_args[] = {'a', 'b', 'c', 'd', 'e', 'f', '\0', 6, -9, 1, 5, 22, 1, 2, 3, 4};
  ASSERT(memcmp(expected_args, list_at(assembler->data_array, 0), sizeof(expected_args)));

  return EXIT_SUCCESS;
}
