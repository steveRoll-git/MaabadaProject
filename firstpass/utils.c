#include "./data.h"
#include <ctype.h>
#include <string.h>

static const struct instruction_t arr[] = {
    {INSTRUCTION_MOV, OPCODE_MOV, TWO_ARGS},
    {INSTRUCTION_CMP, OPCODE_CMP, TWO_ARGS},
    {INSTRUCTION_ADD, OPCODE_ADD, TWO_ARGS},
    {INSTRUCTION_SUB, OPCODE_SUB, TWO_ARGS},
    {INSTRUCTION_NOT, OPCODE_NOT, TWO_ARGS},
    {INSTRUCTION_CLR, OPCODE_CLR, ONE_ARG},
    {INSTRUCTION_LEA, OPCODE_LEA, ONE_ARG},
    {INSTRUCTION_INC, OPCODE_INC, ONE_ARG},
    {INSTRUCTION_DEC, OPCODE_DEC, ONE_ARG},
    {INSTRUCTION_JMP, OPCODE_JMP, ONE_ARG},
    {INSTRUCTION_BNE, OPCODE_BNE, ONE_ARG},
    {INSTRUCTION_RED, OPCODE_RED, ONE_ARG},
    {INSTRUCTION_PRN, OPCODE_PRN, ONE_ARG},
    {INSTRUCTION_JSR, OPCODE_JSR, ONE_ARG},
    {INSTRUCTION_RTS, OPCODE_RTS, NO_ARGS},
    {INSTRUCTION_STOP, OPCODE_STOP, NO_ARGS}};

const int keywords_length = sizeof(arr) / sizeof(struct instruction_t);
/*
 * Returns The correspoding binary value of a string keyword.
 * if the keyword given is NOT a known keyword, return -1.
 */
int keyword_to_value(char *token) {
  if (token == NULL)
    return -1;

  for (int i = 0; i < keywords_length; i++) {
    if (!strcmp(token, arr[i].name))
      return arr[i].value;
  }
  return -1;
}

int is_assembly_command(char *token) {
  if (token == NULL)
    return -1;

  for (int i = 0; i < keywords_length; i++) {
    if (!strcmp(token, arr[i].name))
      return 1;
  }
  return 0;
}

int is_label(char *token) {
  if (token == NULL)
    return -1;

  int length = strlen(token);

  return token[length - 1] == ':';
}

int is_label_valid(char *label) {
  /*Check1: Check if the FIRST character of the label is alphanumeric
   * character*/
  if (!(isalpha(*label)))
    return -1;

  /*TODO: Check2: Check if label isn't already used as macro name*/

  /*TODO: Check3: Label name must be shown Once and only once.*/

  /*Check4: Label can't be a keyword name*/
  if (is_assembly_command(label) == 1)
    return -1;

  return 0;
}
