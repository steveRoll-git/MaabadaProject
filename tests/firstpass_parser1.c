#include "common.h"

int main(int argc, char *argv[]) {
  char *in = "../examples/dummy2/main.am", *out = "../examples/dummy2/main.as";
  int status_code = first_pass(in, out);

  return status_code;
}
