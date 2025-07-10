#include "../include/parser.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../include/data.h"
#include "../include/errors.h"

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s) {
  while (*s != NULL && isspace(**s)) {
    (*s)++;
  }
}

/* Returns whether there are no more non-space characters in `s`. */
int is_end(char *s) {
  skip_spaces(&s);
  return *s == 0;
}

/* Skips leading spaces, and checks the current character under `s`. */
/* If it is equal to `c`, advances and returns TRUE, otherwise returns FALSE. */
bool_t accept(char **s, char c) {
  skip_spaces(s);
  if (**s == c) {
    (*s)++;
    return TRUE;
  }
  return FALSE;
}

/* Given an instruction's name, returns information about that instruction. */
/* If no instruction by that name exists, returns NULL. */
instruction_info_t *get_instruction(char *token) {
  int i;
  if (token == NULL) {
    return NULL;
  }

  for (i = 0; i < num_instructions; i++) {
    if (strcmp(token, instructions[i].name) == 0) {
      return &instructions[i];
    }
  }

  return NULL;
}

/* Copies a character from the string in `src` to the string in `dest`, and advances both of them by one character. */
void add_char(char **dest, char **src) {
  **dest = **src;
  (*dest)++;
  (*src)++;
}

/* Reads the next word (a sequence of alphanumeric characters that starts with a letter) at the string pointed to by
 * `s` */
/* Updates `s` so that it will point to the next character after the word that was read. */
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

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
result_t parse_number(char **s, machine_word_t *result) {
  int next;
  int scanned_values = sscanf(*s, "%d%n", result, &next);
  if (scanned_values == 1) {
    *s += next;
    return SUCCESS;
  }
  return ERR_NUMBER_NOT_VALID;
}

/* Parses the index accessors of a matrix operand, right after the label name. */
result_t parse_matrix_operand(char **s, operand_t *operand) {
  ASSERT(accept(s, '['), ERR_MATRIX_START_BRACKET_ROW)

  {
    word_t word = read_word(s);
    ASSERT(word.kind == WORD_REGISTER, ERR_WRONG_MATRIX_ACCESS)
    operand->data.matrix.row_reg = word.register_index;
  }

  ASSERT(accept(s, ']'), ERR_MATRIX_END_BRACKET_ROW);
  ASSERT(accept(s, '['), ERR_MATRIX_START_BRACKET_COL);

  {
    word_t word = read_word(s);
    ASSERT(word.kind == WORD_REGISTER, ERR_WRONG_MATRIX_ACCESS)
    operand->data.matrix.col_reg = word.register_index;
  }

  ASSERT(accept(s, ']'), ERR_MATRIX_END_BRACKET_COL)

  return SUCCESS;
}

/* Parses an operand and stores it in the given pointer. */
result_t parse_operand(char **s, operand_t *operand) {
  word_t word;

  skip_spaces(s);

  ASSERT(**s != '\0', ERR_NO_ARG);

  /* Immediate number */
  if (accept(s, '#')) {
    machine_word_t value;

    ASSERT(isdigit(**s) || **s == '-' || **s == '+', ERR_NUMBER_AFTER_HASH);
    TRY(parse_number(s, &value))

    operand->kind = OPERAND_KIND_IMMEDIATE;
    operand->data.immediate = value;
    return SUCCESS;
  }

  word = read_word(s);

  if (word.kind == WORD_REGISTER) {
    operand->kind = OPERAND_KIND_REGISTER;
    operand->data.register_index = word.register_index;
    return SUCCESS;
  }

  ASSERT(word.kind == WORD_IDENTIFIER, ERR_INVALID_ARGUMENT);

  skip_spaces(s);

  if (**s == '[') {
    operand->kind = OPERAND_KIND_MATRIX;
    strcpy(operand->data.matrix.label, word.value);
    return parse_matrix_operand(s, operand);
  }

  operand->kind = OPERAND_KIND_LABEL;
  strcpy(operand->data.label, word.value);
  return SUCCESS;
}

/* Parses zero, one or two operands, right after an instruction. */
result_t parse_instruction_operands(char *s, instruction_t *instruction) {
  instruction->num_args = NO_ARGS;
  if (!is_end(s)) {
    TRY(parse_operand(&s, &instruction->operand_1))
    instruction->num_args = ONE_ARG;
    if (accept(&s, ',')) {
      TRY(parse_operand(&s, &instruction->operand_2))
      instruction->num_args = TWO_ARGS;
    }
  }
  ASSERT(is_end(s), ERR_EXTRANEOUS_TEXT)
  return SUCCESS;
}

/* Parses a list of numbers separated by commas, and stores them in the directive's data array. */
result_t parse_data(char *s, directive_t *directive) {
  int *size = &directive->info.data.size;
  *size = 0;

  do {
    skip_spaces(&s);

    TRY(parse_number(&s, &directive->info.data.array[*size]));

    (*size)++;
  }
  while (accept(&s, ','));

  return SUCCESS;
}

/* Parses a string delimited by quotation marks, and stores its contents in the directive's data array. */
result_t parse_string(char *s, directive_t *directive) {
  char *last_quotes;
  int *size = &directive->info.data.size;

  *size = 0;

  /* .string directive must contain a string enclosed in quotes. */
  ASSERT(accept(&s, '"'), ERR_STRING_MISSING_QUOTE_START);

  last_quotes = strrchr(s, '"');
  /* The string must be enclosed by two quotes. */
  ASSERT(last_quotes, ERR_STRING_MISSING_QUOTE_END)
  /* Extraneous text after string. */
  ASSERT(is_end(last_quotes + 1), ERR_EXTRANEOUS_TEXT);

  /*TODO: Do we need to check if all characters are valid ASCII?*/
  while (s < last_quotes) {
    directive->info.data.array[*size] = *s;
    (*size)++;
    s++;
  }
  directive->info.data.array[*size] = 0;
  (*size)++;

  return SUCCESS;
}

/* Parses the arguments of the .mat directive. */
result_t parse_matrix(char *s, directive_t *directive) {
  machine_word_t rows, cols;
  int max_elements;
  int *size = &directive->info.data.size;
  int i = 0;

  *size = 0;

  ASSERT(accept(&s, '['), ERR_MATRIX_START_BRACKET_ROW)
  /* Expected a number. */
  TRY(parse_number(&s, &rows))
  /* Expected ']'. */
  ASSERT(accept(&s, ']'), ERR_MATRIX_END_BRACKET_ROW)
  /* Expected '['. */
  ASSERT(accept(&s, '['), ERR_MATRIX_START_BRACKET_COL)
  /* Expected a number. */
  TRY(parse_number(&s, &cols))
  /* Expected ']'. */
  ASSERT(accept(&s, ']'), ERR_MATRIX_END_BRACKET_COL)

  /* The number of rows and columns must be positive. */
  ASSERT(rows > 0 && cols > 0, ERR_MATRIX_NEGATIVE_STORAGE);
  max_elements = rows * cols;

  skip_spaces(&s);
  if (!is_end(s)) {
    TRY(parse_data(s, directive))
    ASSERT(*size <= max_elements, ERR_MATRIX_OVERFLOW)
  }

  /* Add zeroes for any elements that weren't given. */
  for (i = *size; i < max_elements; i++) {
    directive->info.data.array[*size] = 0;
    (*size)++;
  }

  return SUCCESS;
}

/* Returns which directive the word under `s` represents. */
directive_kind_t read_directive_kind(char **s) {
  /* This buffer is 2 characters longer than the directive to catch cases where the string differs by a single character
   * at the end. */
  char token[DIRECTIVE_MAX_LEN + 2];
  char *last = token;
  int len = 0;

  while (isalpha(**s) && len < DIRECTIVE_MAX_LEN + 1) {
    add_char(&last, s);
    len++;
  }
  *last = 0;

  if (strcmp(token, DIRECTIVE_DATA) == 0) {
    return DIRECTIVE_KIND_DATA;
  }
  if (strcmp(token, DIRECTIVE_STRING) == 0) {
    return DIRECTIVE_KIND_STRING;
  }
  if (strcmp(token, DIRECTIVE_MAT) == 0) {
    return DIRECTIVE_KIND_MAT;
  }
  if (strcmp(token, DIRECTIVE_ENTRY) == 0) {
    return DIRECTIVE_KIND_ENTRY;
  }
  if (strcmp(token, DIRECTIVE_EXTERN) == 0) {
    return DIRECTIVE_KIND_EXTERN;
  }

  return DIRECTIVE_KIND_UNKNOWN;
}

/* Parses a statement, which may be a blank line, an instruction, or a directive. */
result_t parse_statement(char *line, statement_t *statement) {
  word_t word;

  statement->has_label = FALSE;

  skip_spaces(&line);

  if (*line == 0 || *line == ';') {
    /* Entire line of whitespace, ignore. */
    statement->kind = STATEMENT_EMPTY;
    return SUCCESS;
  }

  word = read_word(&line);

  if (word.kind != WORD_NONE && accept(&line, ':')) {
    ASSERT(word.kind == WORD_IDENTIFIER, ERR_INVALID_LABEL);
    ASSERT(strchr(word.value, '_') == NULL, ERR_LABEL_UNDERSCORES)
    ASSERT(strlen(word.value) + 1 <= MAX_LABEL, ERR_LABEL_TOO_LONG)

    statement->has_label = TRUE;
    strcpy(statement->label, word.value);
    word = read_word(&line);
  }

  if (word.kind == WORD_NONE && accept(&line, '.')) {
    directive_kind_t kind = read_directive_kind(&line);

    statement->kind = STATEMENT_DIRECTIVE;
    statement->data.directive.kind = kind;

    switch (kind) {
      case DIRECTIVE_KIND_DATA:
        return parse_data(line, &statement->data.directive);
      case DIRECTIVE_KIND_STRING:
        return parse_string(line, &statement->data.directive);
      case DIRECTIVE_KIND_MAT:
        return parse_matrix(line, &statement->data.directive);
      case DIRECTIVE_KIND_ENTRY:
        /* TODO */
      case DIRECTIVE_KIND_EXTERN:
        /* TODO */
      default:
        return ERR_UNKNOWN_DIRECTIVE;
    }
    /*TODO: external, entry types??*/
  }

  if (word.kind == WORD_INSTRUCTION) {
    statement->kind = STATEMENT_INSTRUCTION;
    statement->data.instruction.info = word.instruction_info;
    TRY(parse_instruction_operands(line, &statement->data.instruction))
    return SUCCESS;
  }

  return ERR_UNKNOWN_INSTRUCTION;
}
