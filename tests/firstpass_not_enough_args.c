#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/wont_work/notEnoughArgs.am", *out = "../examples/wont_work/notEnoughArgs.as";

  int status_code = first_pass(in, out);

  return !status_code;
}
