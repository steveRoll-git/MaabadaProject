#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/dummy2/main.am";
  table_t *macro_table;
  table_create(0, &macro_table);
  context_t *assembler;
  context_create(in, macro_table, &assembler);
  result_t result = codegen(assembler);

  ASSERT(result == SUCCESS)

  return EXIT_SUCCESS;
}
