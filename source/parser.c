#include "../include/parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/data.h"
#include "../include/errors.h"

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s) {
  while (isspace(**s) && **s != '\n') {
    (*s)++;
  }
}

/* Returns whether there are no more non-space characters in the newline/null-terminated string `s`. */
int is_end(char *s) {
  skip_spaces(&s);
  return *s == 0 || *s == '\n';
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

/* In the newline/null-terminated string `s`, returns the last pointer where quotation marks appear, or NULL if they do
 * not appear in the line. */
char *find_last_quotes(char *s) {
  char *last = NULL;
  while (*s && *s != '\n') {
    if (*s == '"') {
      last = s;
    }
    s++;
  }
  return last;
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
void read_word(char **s, word_t *word) {
  char *value_ptr = word->value;

  /* Skip past leading spaces. */
  skip_spaces(s);

  if (!isalpha(**s)) {
    word->kind = WORD_NONE;
    return;
  }

  word->kind = WORD_IDENTIFIER;

  /* Copy characters from `str` to `token`'s value as long as they're not spaces or null. */
  while (isalnum(**s) || **s == '_') {
    add_char(&value_ptr, s);
  }
  *value_ptr = 0;

  if (value_ptr - word->value == 2 && word->value[0] == 'r' && word->value[1] >= '0' && word->value[1] <= '7') {
    /* If the token is 2 characters long, and it's the letter 'r' and a digit from 0 to 7, it's a register name. */
    word->kind = WORD_REGISTER;
    word->register_index = word->value[1] - '0';
  }
  else if (strcmp(word->value, "mcro") == 0) {
    word->kind = WORD_MCRO;
  }
  else if (strcmp(word->value, "mcroend") == 0) {
    word->kind = WORD_MCROEND;
  }
  else {
    instruction_info_t *instruction = get_instruction(word->value);
    if (instruction) {
      word->kind = WORD_INSTRUCTION;
      word->instruction_info = instruction;
    }
  }
}

read_line_status_t read_line(FILE *file, char line[MAX_LINE]) {
  int count = 0;
  int c;
  if (feof(file)) {
    return READ_LINE_EOF;
  }
  while ((c = getc(file)) != EOF) {
    line[count] = (char) c;
    if (c == '\n') {
      return READ_LINE_SUCCESS;
    }
    if (count >= MAX_LINE) {
      /* The line is longer than 80 characters - read everything until the next newline and return 0. */
      while ((c = getc(file)) != EOF && c != '\n') {
      }
      return READ_LINE_TOO_LONG;
    }
    count++;
  }
  line[count] = 0;
  return READ_LINE_SUCCESS;
}

/* Skips leading spaces and parses an integer (with an optional + or -), and stores it in `result`. */
/* May fail if the syntax of the number is incorrect. */
result_t parse_number(char **s, machine_word_t *out) {
  char *orig = *s;
  long result = strtol(*s, s, 10);

  if (*s == orig) {
    /* If `s` is at the same position as when parsing began, then `strtol` did not parse a number. */
    return ERR_NUMBER_NOT_VALID;
  }

  if (**s == '_' || isalpha(**s)) {
    /* If an underscore or letter appears right after the number, it's not a valid number. */
    return ERR_NUMBER_NOT_VALID;
  }

  *out = (machine_word_t) result;
  return SUCCESS;
}

/* Parses the index accessors of a matrix operand, right after the label name. */
/* May fail if the syntax of the operand is incorrect. */
result_t parse_matrix_operand(char **s, operand_t *operand) {
  word_t word;

  ASSERT(accept(s, '['), ERR_MATRIX_START_BRACKET_ROW)

  {
    /* The word in between the first '[]' is the row, and must be a register. */
    read_word(s, &word);
    ASSERT(word.kind == WORD_REGISTER, ERR_WRONG_MATRIX_ACCESS)
    operand->data.matrix.row_reg = word.register_index;
  }

  ASSERT(accept(s, ']'), ERR_MATRIX_END_BRACKET_ROW);
  ASSERT(accept(s, '['), ERR_MATRIX_START_BRACKET_COL);

  {
    /* The word in between the second '[]' is the column, and must be a register. */
    read_word(s, &word);
    ASSERT(word.kind == WORD_REGISTER, ERR_WRONG_MATRIX_ACCESS)
    operand->data.matrix.col_reg = word.register_index;
  }

  ASSERT(accept(s, ']'), ERR_MATRIX_END_BRACKET_COL)

  return SUCCESS;
}

/* Parses an operand and stores it in the given pointer. */
/* May fail if the syntax of the operand is incorrect. */
result_t parse_operand(char **s, operand_t *operand) {
  word_t word;

  skip_spaces(s);

  ASSERT(**s != '\0', ERR_NO_ARG);

  if (accept(s, '#')) {
    /* Operands that begin with a '#' are immediate number operands. */

    machine_word_t value;

    /* `parse_number` skips leading spaces, so we must verify that the text right after the `#` starts a number. */
    ASSERT(isdigit(**s) || **s == '-' || **s == '+', ERR_NUMBER_AFTER_HASH);
    TRY(parse_number(s, &value))

    operand->kind = OPERAND_KIND_IMMEDIATE;
    operand->data.immediate = value;
    return SUCCESS;
  }

  read_word(s, &word);

  if (word.kind == WORD_REGISTER) {
    /* Operands r0-r7 are register operands. */

    operand->kind = OPERAND_KIND_REGISTER;
    operand->data.register_index = word.register_index;
    return SUCCESS;
  }

  /* If none of the above conditions passed, the operand may be a label or a matrix access. */
  /* Both of those must begin with an identifier. */
  ASSERT(word.kind == WORD_IDENTIFIER, ERR_INVALID_ARGUMENT);
  ASSERT(strlen(word.value) + 1 <= MAX_LABEL, ERR_LABEL_TOO_LONG)

  skip_spaces(s);

  /* If there is a '[' right after the label, then it's a matrix operand. */
  if (**s == '[') {
    operand->kind = OPERAND_KIND_MATRIX;
    strcpy(operand->data.matrix.label, word.value);
    return parse_matrix_operand(s, operand);
  }

  /* Otherwise, the operand is a label. */
  operand->kind = OPERAND_KIND_LABEL;
  strcpy(operand->data.label, word.value);
  return SUCCESS;
}

/* Parses between zero and two operands, right after an instruction. */
/* May fail if the syntax of the operands is incorrect. */
result_t parse_instruction_operands(char *s, instruction_t *instruction) {
  instruction->num_args = NO_ARGS;
  if (!is_end(s)) {
    /* If more text after the instruction is present, parse the first operand. */
    TRY(parse_operand(&s, &instruction->operand_1))
    instruction->num_args = ONE_ARG;

    if (accept(&s, ',')) {
      /* If there is a comma after the first operand, parse the second operand. */
      TRY(parse_operand(&s, &instruction->operand_2))
      instruction->num_args = TWO_ARGS;
    }
  }

  /* No more text must be present after the operands have been parsed. */
  ASSERT(is_end(s), ERR_EXTRANEOUS_TEXT_INSTRUCTION)
  return SUCCESS;
}

/* Parses a list of numbers separated by commas, and stores them in the directive's data array. */
/* May fail if the syntax of the parameter is incorrect. */
result_t parse_data(char *s, directive_t *directive) {
  int *size = &directive->info.data.size;
  *size = 0;

  /* At least one number must appear in `.data`, so this loop runs at least once. */
  do {
    TRY(parse_number(&s, &directive->info.data.array[*size]));
    (*size)++;
  }
  while (accept(&s, ','));

  ASSERT(is_end(s), ERR_EXTRANEOUS_TEXT_DIRECTIVE)

  return SUCCESS;
}

/* Parses a string delimited by quotation marks, and stores its contents in the directive's data array. */
/* May fail if the syntax of the parameter is incorrect. */
result_t parse_string(char *s, directive_t *directive) {
  char *last_quotes;
  int *size = &directive->info.data.size;

  *size = 0;

  /* .string directive must contain a string enclosed in quotes. */
  ASSERT(accept(&s, '"'), ERR_STRING_MISSING_QUOTE_START);

  last_quotes = find_last_quotes(s);
  /* The string must be enclosed by two quotes. */
  ASSERT(last_quotes, ERR_STRING_MISSING_QUOTE_END)
  /* Extraneous text after string. */
  ASSERT(is_end(last_quotes + 1), ERR_EXTRANEOUS_TEXT_DIRECTIVE);

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

/* Parses the arguments of the .mat directive, and stores its contents in the directive's data array. */
/* May fail if the syntax of the parameter is incorrect. */
result_t parse_matrix(char *s, directive_t *directive) {
  machine_word_t rows, cols;
  int max_elements;
  int *size = &directive->info.data.size;
  int i = 0;

  *size = 0;

  /* Expected '['. */
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
    /* If more text is present, parse numbers separated by columns - just like in `.data`. */
    TRY(parse_data(s, directive))
    /* Matrix can't contain more values than the given size. */
    ASSERT(*size <= max_elements, ERR_MATRIX_OVERFLOW)
  }

  /* Add zeroes for any elements that weren't given. */
  for (i = *size; i < max_elements; i++) {
    directive->info.data.array[*size] = 0;
    (*size)++;
  }

  return SUCCESS;
}

/* Parses a single label name - the parameter for the .entry and .extern directives. */
/* May fail if the syntax of the parameter is incorrect. */
result_t parse_label_param(char *s, directive_t *directive) {
  word_t word;
  read_word(&s, &word);
  ASSERT(word.kind == WORD_IDENTIFIER, ERR_DIRECTIVE_EXPECTED_LABEL)
  strcpy(directive->info.label, word.value);

  ASSERT(is_end(s), ERR_EXTRANEOUS_TEXT_DIRECTIVE)

  return SUCCESS;
}

/* Returns which directive the word under `s` represents, and moves `s` to point past that word. */
directive_kind_t read_directive_kind(char **s) {
  /* This buffer is 2 characters longer than the directive to catch cases where the string differs by a single character
   * at the end. */
  char token[DIRECTIVE_MAX_LEN + 2];
  char *last = token;
  int len = 0;

  /* Copy the next word in `s` into `token`. */
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

result_t parse_statement(char *line, statement_t *statement) {
  word_t word;

  statement->has_label = FALSE;

  if (*line == ';') {
    /* If the line starts with a `;`, it's a comment, and the rest of it is ignored. */
    statement->kind = STATEMENT_EMPTY;
    return SUCCESS;
  }

  skip_spaces(&line);

  if (*line == 0 || *line == '\n') {
    /* If, after skipping spaces, we reached the end of the line (null terminator or newline), it means it's empty. */
    statement->kind = STATEMENT_EMPTY;
    return SUCCESS;
  }

  read_word(&line, &word);

  if (word.kind != WORD_NONE && *line == ':') {
    /* If there is a word in the beginning that's immediately followed by a ':', it's a label. */

    /* Labels may only be identifiers. */
    ASSERT(word.kind == WORD_IDENTIFIER, ERR_INVALID_LABEL)
    /* Labels may not contain underscores. */
    ASSERT(strchr(word.value, '_') == NULL, ERR_LABEL_UNDERSCORES)
    /* Labels may not be longer than 30 characters. */
    ASSERT(strlen(word.value) + 1 <= MAX_LABEL, ERR_LABEL_TOO_LONG)

    statement->has_label = TRUE;
    strcpy(statement->label, word.value);
    /* Move past the `:`. */
    line++;
    read_word(&line, &word);
  }

  if (word.kind == WORD_NONE && accept(&line, '.')) {
    /* If the line starts with a `.`, it's a directive. */

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
      case DIRECTIVE_KIND_EXTERN:
        /* Both .entry and .extern just need one label as a parameter. */
        return parse_label_param(line, &statement->data.directive);
      default:
        return ERR_UNKNOWN_DIRECTIVE;
    }
  }

  if (word.kind == WORD_INSTRUCTION) {
    /* If the line begins with an instruction, parse the operands. */
    statement->kind = STATEMENT_INSTRUCTION;
    statement->data.instruction.info = word.instruction_info;
    TRY(parse_instruction_operands(line, &statement->data.instruction))
    return SUCCESS;
  }

  /* A line cannot begin with anything else - return an error. */
  return ERR_UNKNOWN_INSTRUCTION;
}
