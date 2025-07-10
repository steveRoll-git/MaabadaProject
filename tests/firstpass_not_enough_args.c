#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/wont_work/notEnoughArgs.am", *out = "../examples/wont_work/notEnoughArgs.as";
  assembler_t *assembler = assembler_create();
  int status_code = first_pass(in, out, assembler);

  return !status_code;
}
