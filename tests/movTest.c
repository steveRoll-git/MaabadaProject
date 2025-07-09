#include "../include/data.h"
instruction_info_t *get_instruction(char *token);

int main() {
  instruction_info_t *a = get_instruction("mov");
  return a->opcode == OPCODE_MOV ? 0 : 1;
}
