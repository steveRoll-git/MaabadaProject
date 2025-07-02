#include "../common/utils.h"

int main() {
  int a = keyword_to_value("mov");
  return a == OPCODE_MOV ? 0 : 1;
}
