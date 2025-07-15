#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/dummy2/main.am";
  table_t *macro_table;
  table_create(0, &macro_table);
  assembler_t *assembler;
  assembler_create(in, macro_table, &assembler);
  bool_t success = codegen(assembler);

  ASSERT(success)

  return EXIT_SUCCESS;
}
