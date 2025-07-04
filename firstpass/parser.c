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
int accept(char **s, char c) {
  skip_spaces(s);
  if (**s == c) {
    (*s)++;
    return 1;
  }
  return 0;
}

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
int parse_int(char **s, int *result) {
  int next;
  int scanned_values = sscanf(*s, "%d%n", result, &next);
  if (scanned_values == 1) {
    *s += next;
    return 1;
  }
  return 0;
}

int parse_matrix_operand(char **s) {
  int row_reg, col_reg, size;

  size = identifier_length(*s);
  *s += size;

  ASSERT(accept(s, '['));

  ASSERT(is_register(*s));
  row_reg = *(*s + 1) - '0';
  *s += 2;

  ASSERT(accept(s, ']'));
  ASSERT(accept(s, '['));


  ASSERT(is_register(*s));

  col_reg = *(*s + 1) - '0';
  *s += 2;

  ASSERT(accept(s, ']'));

  return 1;
}

bool_t is_mat(char *s) {
  if (s == NULL) {
    return FALSE;
  }

  skip_spaces(&s);

  int size = identifier_length(s);

  s += size;

  skip_spaces(&s);
  return *s == '[';
}

/* Changes PTR, doesn't Return error codes (the way parse_int works), it's a building block for every other function.*/
operand_kind_t parse_instruction_argument(char **s, assembler_t *assembler) {
  int temp;
  char *ptr;
  word_t word;

  skip_spaces(s);

  if (**s == '\0') {
    return OPERAND_KIND_INVALID;
  }

  /* Immediate number */
  if (accept(s, '#')) {
    if (!isdigit(**s) && **s != '-' && **s != '+') {
      return OPERAND_KIND_INVALID;
    }

    if (!parse_int(s, &temp)) {
      fprintf(stderr, "Error parsing instruction number.");
      return OPERAND_KIND_INVALID;
    }

    return OPERAND_KIND_WHOLE_NUMBER;
  }

  word = read_word(s);

  if (word.kind == WORD_REGISTER) {
    return OPERAND_KIND_REGISTER;
  }

  if (word.kind != WORD_IDENTIFIER) {
    return OPERAND_KIND_INVALID;
  }

  skip_spaces(s);

  if (**s == '[') {
    if (!parse_matrix_operand(s)) {
      return OPERAND_KIND_INVALID;
    }
    return OPERAND_KIND_MATRIX;
  }

  return OPERAND_KIND_LABEL;
}

int get_word_size(operand_kind_t arg1, operand_kind_t arg2) {
  int size1 = 1, size2 = 1;
  if (arg1 == OPERAND_KIND_REGISTER && arg2 == OPERAND_KIND_REGISTER) {
    return 1;
  }

  if (arg1 == OPERAND_KIND_MATRIX) {
    size1 = 2;
  }

  if (arg2 == OPERAND_KIND_MATRIX) {
    size2 = 2;
  }

  else if (arg2 == OPERAND_KIND_INVALID) {
    size2 = 0;
  }

  return size1 + size2;
}

int parse_instruction_args(char **s, const args_t args, assembler_t *assembler) {
  skip_spaces(s);
  operand_kind_t arg1, arg2;
  int size;
  assembler->ic += 1;

  switch (args) {
    case NO_ARGS:
      ASSERTM((*s == NULL || **s == '\0'), ERR_EXTRANOUS_INFORMATION_AFTER_ARGUEMENTS);
      return TRUE;


    case ONE_ARG:
      arg1 = parse_instruction_argument(s, assembler);
      ASSERTM(arg1 != OPERAND_KIND_INVALID, ERR_FIRST_ARG_INVALID);

      size = get_word_size(arg1, OPERAND_KIND_INVALID);
      assembler->ic += size;

      skip_spaces(s);
      ASSERTM((*s == NULL || **s == '\0'), ERR_EXTRANOUS_INFORMATION_AFTER_ARGUEMENTS);

      return TRUE;

    case TWO_ARGS:
      arg1 = parse_instruction_argument(s, assembler);
      ASSERTM(arg1 != OPERAND_KIND_INVALID, ERR_FIRST_ARG_INVALID);

      ASSERTM(accept(s, ','), ERR_WHERE_IS_MY_COMMA);

      arg2 = parse_instruction_argument(s, assembler);
      ASSERTM(arg2 != OPERAND_KIND_INVALID, ERR_SECOND_ARG_INVALID);

      size = get_word_size(arg1, arg2);
      assembler->ic += size;

      ASSERTM((*s == NULL || **s == '\0'), ERR_EXTRANOUS_INFORMATION_AFTER_ARGUEMENTS);

      return TRUE;
  }

  return FALSE;
}

int parse_data(char *s, assembler_t *assembler) {
  do {
    int number;
    skip_spaces(&s);
    if (!parse_int(&s, &number)) {
      printf("Malformed number.\n");
      return 0;
    }
    add_data(assembler, number);
    skip_spaces(&s);
  }
  while (accept(&s, ','));

  return 1;
}

int parse_string(char *s, assembler_t *assembler) {
  char *last_quotes;

  /* .string directive must contain a string enclosed in quotes. */
  ASSERT(accept(&s, '"'))

  last_quotes = strrchr(s, '"');
  /* The string must be enclosed by two quotes. */
  ASSERT(last_quotes)
  /* Extraneous text after string. */
  ASSERT(is_end(last_quotes + 1))

  while (s < last_quotes) {
    add_data(assembler, *s);
    s++;
  }
  add_data(assembler, '\0');

  return 1;
}

int parse_matrix(char *s, assembler_t *assembler) {
  int rows, cols;
  int max_elements;
  int prev_dc = assembler->dc;
  int i;
  skip_spaces(&s);
  ASSERT(accept(&s, '['))
  /* Expected a number. */
  ASSERT(parse_int(&s, &rows))
  /* Expected ']'. */
  ASSERT(accept(&s, ']'))
  /* Expected '['. */
  ASSERT(accept(&s, '['))
  /* Expected a number. */
  ASSERT(parse_int(&s, &cols))
  /* Expected ']'. */
  ASSERT(accept(&s, ']'))

  /* The number of rows and columns must be positive. */
  ASSERT(rows <= 0 || cols <= 0)
  max_elements = rows * cols;

  skip_spaces(&s);
  if (!is_end(s)) {
    if (!parse_data(s, assembler)) {
      return 0;
    }

    ASSERTM(assembler->dc - (prev_dc + 1) <= max_elements, ERR_MATRIX_OVERFLOW)
  }

  /* TODO get clarification on forums about this */
  /* Add zeroes for any elements that weren't given. */
  for (i = assembler->dc - prev_dc; i < max_elements; i++) {
    add_data(assembler, 0);
  }

  return 1;
}

directive_kind_t get_directive_kind(char *token) {
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

int compile_assembly_code(char *line, assembler_t *assembler) {
  char *temp, *rest;
  int label_flag = 0;

  temp = strtok(line, " \t");

  /*If we get Whitespace, or a comment*/
  if (temp == NULL || *temp == 0 || *temp == ';') {
    /*Entire line of whitespace, ignore.*/
    return 1;
  }


  /*From here, we can 100% be sure we either have an instruction command, or
   * data command.*/

  /*Do we have a label?*/
  int is_label_flag = is_label(temp);

  if (is_label_flag) {
    ASSERTM(is_label_valid(temp, assembler), INVALID_LABEL_ERR);

    // else
    /*TODO: Need to know if its Data or Intruction before setting label.*/
    list_add(&assembler->label_table, temp, assembler->ic);
    temp = strtok(NULL, " \t");
  }

  rest = strtok(NULL, "");

  if (rest == NULL) {
    rest = "\0";
  }

  /*Is my token an instruction, or data label?*/
  /*If there's a dot, it means it's a data command.*/

  /*TODO: All the functions here parse_data, mat, etc must return the size of DC values entered, so we can set up label
   * correctly.*/
  if (*temp == '.') {
    directive_kind_t kind = get_directive_kind(temp);
    switch (kind) {
      case DIRECTIVE_KIND_DATA:
        return parse_data(rest, assembler);
      case DIRECTIVE_KIND_STRING:
        return parse_string(rest, assembler);
      case DIRECTIVE_KIND_MAT:
        return parse_matrix(rest, assembler);
      case DIRECTIVE_KIND_ENTRY:
        /* TODO */
      case DIRECTIVE_KIND_EXTERN:
        /* TODO */
      case DIRECTIVE_KIND_UNKNOWN:
        fprintf(stderr, "Unknown datatype.");
        return 0;
    }
    /*TODO: external, entry types??*/
  }

  else if (is_assembly_instruction(temp)) {
    instruction_t *instruction = get_instruction(temp);

    ASSERT(parse_instruction_args(&rest, instruction->arg_amount, assembler))
  }
  else {
    fprintf(stderr, "Invalid command");
    return 0;
  }

  return 1;
}
