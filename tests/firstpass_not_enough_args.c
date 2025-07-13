#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/wont_work/notEnoughArgs.am";
  assembler_t *assembler = assembler_create(in, table_create(0));
  bool_t success = codegen(assembler);

  ASSERT(!success)

  return EXIT_SUCCESS;
}
