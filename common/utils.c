#include "./utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "./data.h"

/* This array stores all needed information about all the instructions. */
instruction_info_t instructions[] = {{INSTRUCTION_MOV, OPCODE_MOV, TWO_ARGS},
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

const int num_instructions = sizeof(instructions) / sizeof(struct instruction_info_t);

void add_char(char **dest, char **src) {
  **dest = **src;
  (*dest)++;
  (*src)++;
}

word_t read_word(char **s) {
  word_t word;
  char *value_ptr = word.value;

  /* Skip past leading spaces. */
  skip_spaces(s);

  if (!isalpha(**s)) {
    word.kind = WORD_NONE;
    return word;
  }

  word.kind = WORD_IDENTIFIER;

  /* Copy characters from `str` to `token`'s value as long as they're not spaces or null. */
  while (isalnum(**s) || **s == '_') {
    add_char(&value_ptr, s);
  }
  *value_ptr = 0;

  if (value_ptr - word.value == 2 && word.value[0] == 'r' && word.value[1] >= '0' && word.value[1] <= '7') {
    /* If the token is 2 characters long, and it's the letter 'r' and a digit from 0 to 7, it's a register name. */
    word.kind = WORD_REGISTER;
    word.register_index = word.value[1] - '0';
  }
  else if (strcmp(word.value, "mcro") == 0) {
    word.kind = WORD_MCRO;
  }
  else if (strcmp(word.value, "mcroend") == 0) {
    word.kind = WORD_MCROEND;
  }
  else {
    instruction_info_t *instruction = get_instruction(word.value);
    if (instruction) {
      word.kind = WORD_INSTRUCTION;
      word.instruction_info = instruction;
    }
  }

  return word;
}

instruction_info_t *get_instruction(char *token) {
  int i;
  if (token == NULL) {
    return NULL;
  }

  for (i = 0; i < num_instructions; i++) {
    if (!strcmp(token, instructions[i].name)) {
      return &instructions[i];
    }
  }

  return NULL;
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
      return SENTENCE_ERR_BUFF_OVERFLOW;
    }
    line[count] = (char) c;
    count++;
  }
  line[count] = 0;
  return SENTENCE_NEW_LINE;
}

void skip_spaces(char **s) {
  while (*s != NULL && isspace(**s)) {
    (*s)++;
  }
}

int is_end(char *s) {
  skip_spaces(&s);
  return *s == 0;
}
