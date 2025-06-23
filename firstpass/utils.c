#include "../common/linked_list.h"
#include "./data.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const struct instruction_t arr[] = {{INSTRUCTION_MOV, OPCODE_MOV, TWO_ARGS},
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

int is_label_valid(char *label, linked_list_t *table_arr[]) {
  /*Check1: Check if the FIRST character of the label is alphanumeric
   * character*/
  int size = sizeof(table_arr) / sizeof(table_arr[0]);
  int i = 0;

  linked_list_t *temp;

  if (!(isalpha(*label)))
    return 0;

  /*Check4: Label can't be a keyword name*/
  if (is_assembly_command(label) == 1)
    return 0;

  for (i = 0; i < size; i++) {
    temp = table_arr[i];
    if (list_get(temp, label) != -1L)
      return 0;
  }

  return 1;
}

/* Reads a single line from the file that is at most `MAX_LINE` bytes long, and
 * stores it in `line`. */
/* Returns 0 if the line is longer than 80 characters, 1 otherwise. */
sentence_t read_line(FILE *file, char line[MAX_LINE]) {
  int count = 0;
  int c;
  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      line[count] = 0;
      return SENTENCE_NEW_LINE;
    }
    if (count >= MAX_LINE) {
      /* The line is longer than 80 characters - read everything until the next
       * newline and return 0. */
      while ((c = getc(file)) != EOF && c != '\n') {
      }
      printf("Error: Line is longer than 80 characters.\n");
      return SENTENCE_ERR_BUFF_OVERFLOW;
    }
    line[count] = (char)c;
    count++;
  }
  line[count] = 0;
  return SENTENCE_EOF;
}
