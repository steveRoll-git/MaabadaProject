#include "../headers/data.h"

#include <string.h>

instruction_info_t *get_instruction(char *name) {
  /* This array contains all needed information about every kind of instruction. */
  static instruction_info_t instructions[] = {
          {INSTRUCTION_MOV,  OPCODE_MOV,  TWO_ARGS, FALSE, TRUE },
          {INSTRUCTION_CMP,  OPCODE_CMP,  TWO_ARGS, TRUE,  TRUE },
          {INSTRUCTION_ADD,  OPCODE_ADD,  TWO_ARGS, FALSE, TRUE },
          {INSTRUCTION_SUB,  OPCODE_SUB,  TWO_ARGS, FALSE, TRUE },
          {INSTRUCTION_LEA,  OPCODE_LEA,  TWO_ARGS, FALSE, FALSE},
          {INSTRUCTION_CLR,  OPCODE_CLR,  ONE_ARG,  FALSE, TRUE },
          {INSTRUCTION_NOT,  OPCODE_NOT,  ONE_ARG,  FALSE, TRUE },
          {INSTRUCTION_INC,  OPCODE_INC,  ONE_ARG,  FALSE, FALSE},
          {INSTRUCTION_DEC,  OPCODE_DEC,  ONE_ARG,  FALSE, FALSE},
          {INSTRUCTION_JMP,  OPCODE_JMP,  ONE_ARG,  FALSE, FALSE},
          {INSTRUCTION_BNE,  OPCODE_BNE,  ONE_ARG,  FALSE, FALSE},
          {INSTRUCTION_JSR,  OPCODE_JSR,  ONE_ARG,  FALSE, FALSE},
          {INSTRUCTION_RED,  OPCODE_RED,  ONE_ARG,  FALSE, FALSE},
          {INSTRUCTION_PRN,  OPCODE_PRN,  ONE_ARG,  TRUE,  FALSE},
          {INSTRUCTION_RTS,  OPCODE_RTS,  NO_ARGS,  FALSE, FALSE},
          {INSTRUCTION_STOP, OPCODE_STOP, NO_ARGS,  FALSE, FALSE}
  };

  /* The total number of instructions. */
  static const int num_instructions = sizeof(instructions) / sizeof(instruction_info_t);

  int i;

  if (name == NULL) {
    return NULL;
  }

  for (i = 0; i < num_instructions; i++) {
    if (strcmp(name, instructions[i].name) == 0) {
      return &instructions[i];
    }
  }

  return NULL;
}
