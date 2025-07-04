#include "../common/utils.h"

int main() {
  instruction_t *a = get_instruction("mov");
  return a->opcode == OPCODE_MOV ? 0 : 1;
}
