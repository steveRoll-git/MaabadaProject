#include "./utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../datatypes/linked_list.h"
#include "./data.h"

const struct instruction_t arr[] = {{INSTRUCTION_MOV, OPCODE_MOV, TWO_ARGS}, {INSTRUCTION_CMP, OPCODE_CMP, TWO_ARGS},
                                    {INSTRUCTION_ADD, OPCODE_ADD, TWO_ARGS}, {INSTRUCTION_SUB, OPCODE_SUB, TWO_ARGS},
                                    {INSTRUCTION_NOT, OPCODE_NOT, TWO_ARGS}, {INSTRUCTION_CLR, OPCODE_CLR, ONE_ARG},
                                    {INSTRUCTION_LEA, OPCODE_LEA, ONE_ARG},  {INSTRUCTION_INC, OPCODE_INC, ONE_ARG},
                                    {INSTRUCTION_DEC, OPCODE_DEC, ONE_ARG},  {INSTRUCTION_JMP, OPCODE_JMP, ONE_ARG},
                                    {INSTRUCTION_BNE, OPCODE_BNE, ONE_ARG},  {INSTRUCTION_RED, OPCODE_RED, ONE_ARG},
                                    {INSTRUCTION_PRN, OPCODE_PRN, ONE_ARG},  {INSTRUCTION_JSR, OPCODE_JSR, ONE_ARG},
                                    {INSTRUCTION_RTS, OPCODE_RTS, NO_ARGS},  {INSTRUCTION_STOP, OPCODE_STOP, NO_ARGS}};

const int keywords_length = sizeof(arr) / sizeof(struct instruction_t);
/*
 * Returns The correspoding binary value of a string keyword.
 * if the keyword given is NOT a known keyword, return -1.
 */
int keyword_to_value(char *token) {
  int i;
  if (token == NULL)
    return -1;

  for (i = 0; i < keywords_length; i++) {
    if (!strcmp(token, arr[i].name))
      return arr[i].value;
  }
  return -1;
}

int is_assembly_command(char *token) {
  int i;
  if (token == NULL)
    return -1;

  for (i = 0; i < keywords_length; i++) {
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

int is_label_valid(char *label, linked_list_t *macro_table, linked_list_t *label_table, linked_list_t *data_table) {
  linked_list_t *temp;
  char *ch = label;
  /* Check if the first character of the label is a letter */

  if (!(isalpha(*label)))
    return 0;


  /*TODO: All Characters AFTER the first character must be [a-zA-Z1-9]+*/
  while (*ch != '\0') {
    if (!isalpha(*ch) && !isdigit(*ch))
      return 0;
    ch++;
  }

  /*TODO: Characters CANT BE Other keywords (R1-8), probably more that im missin.*/
  if (is_register(label))
    return 0;

  /* Label can't be a keyword name */
  if (is_assembly_command(label) == 1)
    return 0;

  /* Label can't be a name of a macro */
  if (list_get(macro_table, label)) {
    return 0;
  }

  /* Label can't be a name of an existing label */
  if (list_get(label_table, label)) {
    return 0;
  }

  if (list_get(data_table, label)) {
    return 0;
  }

  return 1;
}

/* Reads a single line from the file that is at most `MAX_LINE` bytes long, and
 * stores it in `line`. */
/* Returns `SENTENCE_NEW_LINE` if there are more lines to be read, `SENTENCE_ERR_BUFF_OVERFLOW` if the line was too
 * long, and `SENTENCE_EOF` if there are no more lines to read. */
sentence_t read_line(FILE *file, char line[MAX_LINE]) {
  int count = 0;
  int c;
  if (feof(file)) {
    return SENTENCE_EOF;
  }
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
    line[count] = (char) c;
    count++;
  }
  line[count] = 0;
  return SENTENCE_NEW_LINE;
}


int label_size(const char *label) {
  int count = 0;

  if (!isalpha(*label))
    return 0;

  while ((isalpha(*label) || isdigit(*label))) {
    count++;
    label++;
  }
  return count;
}

int is_register(const char *token) {
  if (token == NULL)
    return 0;

  int length = label_size(token);
  char register_number = *(token + 1);

  return length == 2 && *token == 'R' && register_number >= '0' && register_number <= '7';
}
