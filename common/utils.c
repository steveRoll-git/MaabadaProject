#include "./utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "./data.h"

/* This array stores all needed information about all the instructions. */
instruction_t instructions[] = {{INSTRUCTION_MOV, OPCODE_MOV, TWO_ARGS},
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

int does_begin_number(char *s) {
  return isdigit(*s) || ((*s == '-' || *s == '+') && isdigit(*(s + 1)));
}

/* Copies a character from the string in `src` to the string in `dest`, and advances both of them by one character. */
void add_char(char **dest, char **src) {
  **dest = **src;
  (*dest)++;
  (*src)++;
}

word_t read_word(char **s) {
  word_t word;
  char *value_ptr = word.value;

  /* Skip past leading spaces. */
  while (isspace(**s)) {
    (*s)++;
  }

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
    instruction_t *instruction = get_instruction(word.value);
    if (instruction) {
      word.kind = WORD_INSTRUCTION;
      word.instruction = instruction;
    }
  }

  return word;
}

instruction_t *get_instruction(char *token) {
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

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s) {
  while (*s != NULL && isspace(**s)) {
    (*s)++;
  }
}

int is_end(char *s) {
  skip_spaces(&s);
  return *s == 0;
}
