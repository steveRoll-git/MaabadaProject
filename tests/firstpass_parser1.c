#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/dummy2/main.am";
  assembler_t *assembler = assembler_create(table_create(0));
  bool_t success = codegen(in, assembler);

  ASSERT(success)

  return EXIT_SUCCESS;
}
