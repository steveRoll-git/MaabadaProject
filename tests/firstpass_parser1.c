#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/dummy2/main.am", *out = "../examples/dummy2/main.as";
  assembler_t assembler = assembler_create();
  int status_code = first_pass(in, out, &assembler);

  return status_code;
}
