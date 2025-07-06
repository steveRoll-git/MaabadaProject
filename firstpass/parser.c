#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/ERR.h"
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/linked_list.h"

/* If the current character that `s` points to is equal to `c`, advance and return 1, otherwise return 0. */
bool_t accept(char **s, char c) {
  skip_spaces(s);
  if (**s == c) {
    (*s)++;
    return TRUE;
  }
  return FALSE;
}

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
bool_t parse_number(char **s, machine_word_t *result) {
  int next;
  int scanned_values = sscanf(*s, "%d%n", result, &next);
  if (scanned_values == 1) {
    *s += next;
    return TRUE;
  }
  return FALSE;
}

bool_t parse_instruction_operands(char *s, instruction_t *instruction) {
  instruction->num_args = NO_ARGS;
  if (!is_end(s)) {
    ASSERT(parse_operand(&s, &instruction->operand_1))
    instruction->num_args = ONE_ARG;
    if (accept(&s, ',')) {
      ASSERT(parse_operand(&s, &instruction->operand_2))
      instruction->num_args = TWO_ARGS;
    }
  }
  ASSERTM(is_end(s), ERR_EXTRANOUS_INFORMATION_AFTER_ARGUEMENTS)
  return TRUE;
}

bool_t parse_matrix_operand(char **s, operand_t *operand) {
  ASSERT(accept(s, '['));

  {
    word_t word = read_word(s);
    ASSERT(word.kind == WORD_REGISTER)
    operand->data.matrix.row_reg = word.register_index;
  }

  ASSERT(accept(s, ']'));
  ASSERT(accept(s, '['));

  {
    word_t word = read_word(s);
    ASSERT(word.kind == WORD_REGISTER)
    operand->data.matrix.col_reg = word.register_index;
  }

  ASSERT(accept(s, ']'));

  return TRUE;
}

/* Parses an operand and stores it in the given pointer. Returns whether parsing was successful. */
bool_t parse_operand(char **s, operand_t *operand) {
  word_t word;

  skip_spaces(s);

  if (**s == '\0') {
    return FALSE;
  }

  /* Immediate number */
  if (accept(s, '#')) {
    machine_word_t value;

    /* Numbers must start with a digit, '-' or '+'. */
    if (!isdigit(**s) && **s != '-' && **s != '+') {
      return FALSE;
    }

    if (!parse_number(s, &value)) {
      fprintf(stderr, "Error parsing instruction number.");
      return FALSE;
    }

    operand->kind = OPERAND_KIND_IMMEDIATE;
    operand->data.immediate = value;
    return TRUE;
  }

  word = read_word(s);

  if (word.kind == WORD_REGISTER) {
    operand->kind = OPERAND_KIND_REGISTER;
    operand->data.register_index = word.register_index;
    return TRUE;
  }

  if (word.kind != WORD_IDENTIFIER) {
    return FALSE;
  }

  skip_spaces(s);

  if (**s == '[') {
    operand->kind = OPERAND_KIND_MATRIX;
    strcpy(operand->data.matrix.label, word.value);
    return parse_matrix_operand(s, operand);
  }

  operand->kind = OPERAND_KIND_LABEL;
  strcpy(operand->data.label, word.value);
  return TRUE;
}

bool_t parse_data(char *s, directive_t *directive) {
  int *size = &directive->info.data.size;
  *size = 0;
  do {
    skip_spaces(&s);
    if (!parse_number(&s, &directive->info.data.array[*size])) {
      printf("Malformed number.\n");
      return FALSE;
    }
    (*size)++;
  }
  while (accept(&s, ','));

  return TRUE;
}

bool_t parse_string(char *s, directive_t *directive) {
  char *last_quotes;
  int *size = &directive->info.data.size;

  *size = 0;

  /* .string directive must contain a string enclosed in quotes. */
  ASSERTM(accept(&s, '"'), ERR_STRING_MISSING_QUOTE);

  last_quotes = strrchr(s, '"');
  /* The string must be enclosed by two quotes. */
  ASSERTM(last_quotes, ERR_STRING_MISSING_QUOTE)
  /* Extraneous text after string. */
  ASSERTM(is_end(last_quotes + 1), ERR_EXTRANOUS_INFORMATION_AFTER_ARGUEMENTS);

  /*TODO: Do we need to check if all characters are valid ASCII?*/
  while (s < last_quotes) {
    directive->info.data.array[*size] = *s;
    (*size)++;
    s++;
  }
  directive->info.data.array[*size] = 0;
  (*size)++;

  return TRUE;
}

bool_t parse_matrix(char *s, directive_t *directive) {
  machine_word_t rows, cols;
  int max_elements;
  int *size = &directive->info.data.size;
  int i = 0;

  *size = 0;

  ASSERTM(accept(&s, '['), ERR_MATRIX_START_BRACKET_ROW)
  /* Expected a number. */
  ASSERTM(parse_number(&s, &rows), ERR_NUMBER_NOT_VALID)
  /* Expected ']'. */
  ASSERTM(accept(&s, ']'), ERR_MATRIX_END_BRACKET_ROW)
  /* Expected '['. */
  ASSERTM(accept(&s, '['), ERR_MATRIX_START_BRACKET_COL)
  /* Expected a number. */
  ASSERTM(parse_number(&s, &cols), ERR_NUMBER_NOT_VALID)
  /* Expected ']'. */
  ASSERTM(accept(&s, ']'), ERR_MATRIX_END_BRACKET_COL)

  /* The number of rows and columns must be positive. */
  ASSERTM(rows > 0 && cols > 0, ERR_MATRIX_NEGATIVE_STORAGE);
  max_elements = rows * cols;

  skip_spaces(&s);
  if (!is_end(s)) {
    if (!parse_data(s, directive)) {
      return FALSE;
    }

    ASSERTM(*size <= max_elements, ERR_MATRIX_OVERFLOW)
  }

  /* Add zeroes for any elements that weren't given. */
  for (i = *size; i < max_elements; i++) {
    directive->info.data.array[*size] = 0;
    (*size)++;
  }

  return TRUE;
}

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

bool_t parse_statement(char *line, statement_t *statement) {
  word_t word;

  skip_spaces(&line);

  if (*line == 0 || *line == ';') {
    /* Entire line of whitespace, ignore. */
    statement->kind = STATEMENT_EMPTY;
    return TRUE;
  }

  word = read_word(&line);

  if (word.kind != WORD_NONE && accept(&line, ':')) {
    ASSERTM(word.kind == WORD_IDENTIFIER, ERR_INVALID_LABEL);
    ASSERTM(strchr(word.value, '_') == NULL, ERR_LABEL_UNDERSCORES)
    ASSERTM(strlen(word.value) + 1 <= MAX_LABEL, ERR_LABEL_TOO_LONG)

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
        fprintf(stderr, "Unknown directive.\n");
        return FALSE;
    }
    /*TODO: external, entry types??*/
  }

  if (word.kind == WORD_INSTRUCTION) {
    statement->kind = STATEMENT_INSTRUCTION;
    statement->data.instruction.info = word.instruction_info;
    ASSERT(parse_instruction_operands(line, &statement->data.instruction))
    return TRUE;
  }

  fprintf(stderr, ERR_INVALID_COMMAND);
  return FALSE;
}
