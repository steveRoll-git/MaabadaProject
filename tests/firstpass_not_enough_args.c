#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/wont_work/notEnoughArgs.am";
  assembler_t *assembler = assembler_create();
  int status_code = codegen(in, assembler);

  return !status_code;
}
