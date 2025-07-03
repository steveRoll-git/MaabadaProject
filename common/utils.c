#include "./utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "./data.h"

/* This array stores all needed information about all the instructions. */
const instruction_t instructions[] = {{INSTRUCTION_MOV, OPCODE_MOV, TWO_ARGS},
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

const int num_instructions = sizeof(instructions) / sizeof(struct instruction_t);

instruction_t get_instruction(char *token) {
  int i;
  if (token == NULL) {
    return (instruction_t) {NULL, -1, 0};
  }

  for (i = 0; i < num_instructions; i++) {
    if (!strcmp(token, instructions[i].name)) {
      return instructions[i];
    }
  }

  return (instruction_t) {NULL, -1, 0};
}

int is_assembly_instruction(char *token) {
  int i, length;
  if (token == NULL) {
    return -1;
  }

  length = identifier_length(token);

  for (i = 0; i < num_instructions; i++) {
    char *name = instructions[i].name;
    int instruction_len = strlen(name);
    if (length == instruction_len && strncmp(token, name, instruction_len) == 0) {
      return 1;
    }
  }
  return 0;
}

int is_label(char *token) {
  if (token == NULL) {
    return -1;
  }

  int length = strlen(token);

  return token[length - 1] == ':';
}

int is_label_valid(char *label, assembler_t *assembler) {
  linked_list_t *temp;
  char *ch = label;

  /* Check if the first character of the label is a letter */
  if (!(isalpha(*label))) {
    return 0;
  }

  while (*ch != '\0') {
    if (!isalpha(*ch) && !isdigit(*ch)) {
      return 0;
    }
    ch++;
  }

  if (is_register(label)) {
    return 0;
  }

  /* Label can't be a keyword name */
  if (is_assembly_instruction(label) == 1) {
    return 0;
  }

  /* Label can't be a name of a macro */
  if (list_get(&assembler->macro_table, label) != -1L) {
    return 0;
  }

  /* Label can't be a name of an existing label */
  if (list_get(&assembler->label_table, label) != -1L) {
    return 0;
  }

  if (list_get(&assembler->data_table, label) != -1L) {
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

int identifier_length(const char *ident) {
  int count = 0;

  if (!isalpha(*ident)) {
    return 0;
  }

  while (isalpha(*ident) || isdigit(*ident)) {
    count++;
    ident++;
  }
  return count;
}

int is_register(const char *token) {
  if (token == NULL) {
    return 0;
  }

  int length = identifier_length(token);
  char register_number = *(token + 1);

  return length == 2 && *token == 'r' && register_number >= '0' && register_number <= '7';
}
