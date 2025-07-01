#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/linked_list.h"

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s) {
  while (isspace(**s)) {
    (*s)++;
  }
}

/* If the current character that `s` points to is equal to `c`, advance and return 1, otherwise return 0. */
int accept(char **s, char c) {
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

/* Changes PTR, doesn't Return error codes (the way parse_int works), it's a building block for every other function.*/
/*TODO: add status return type code of what you you've returned. (Register, Matrix, Label, WholeNumber)*/
int parse_instruction_arguement(char **s, assembler_t *assembler) {
  int temp;
  /*EXAMPLES:  R1-R8, LABEL, MAT  , *-1 */
  skip_spaces(s);

  if (**s == '\0')
    return 0;

  /* Absolute Number */

  if (**s == '#') {
    (*s)++;

    if (!isdigit(**s) && **s != '-' && **s != '+')
      return 0;

    if (!parse_int(s, &temp)) {
      fprintf(stderr, "Error parsing instruction number.");
      return 0;
    }

    return 1;
  }

  /*
   * Registers
   * Check first letter is R
   * Check second letter is ascii between '0'-'8'
   * Check third letter is an end, Space tab, or a new arg (, ]), I removed , because we need to know if its 0ARG or 2
   * ARG.
   */
  int size = label_size(*s);

  if (is_register(*s)) {
    *s += size;
    return 1;
  }

  /* Must be label,
   * or worse, a label with a matrix addition!
   */


  if (!is_label_valid(*s, assembler))
    return 0;
  /* Skip between all the characters of the label. not important in first pass.*/
  *s += size;

  //
  // if (**s == '[') {
  //   /*  parse_matrix_values(s); TODO: Should return ERROR if the parsing of the matrix isnt correct.*/
  // }
  skip_spaces(s);
  return 1;
}

int parse_matrix_values(char **s, assembler_t *assembler) {
  if (**s != '[')
    return 0;

  (*s)++;
  /*TODO: Only accepts registers.*/
  parse_instruction_arguement(s, assembler);

  if (**s != ']')
    return 0;
  skip_spaces(s);
  return 1;
}

int parse_instruction_args(char **s, const args_t args, assembler_t *assembler) {
  skip_spaces(s);

  switch (args) {
    case NO_ARGS:
      return **s == '\0';


    case ONE_ARG:
      if (!parse_instruction_arguement(s, assembler))
        return 0;
      skip_spaces(s);
      return **s == '\0';

    case TWO_ARGS:
      /*TODO: shrink IC message by 1, when both arguments are registers. */
      parse_instruction_arguement(s, assembler);
      skip_spaces(s);

      if (**s != ',')
        return 0;
      (*s)++;


      skip_spaces(s);
      parse_instruction_arguement(s, assembler);
      skip_spaces(s);
      return **s == '\0';
  }
  /*IF we're here... something really bad must've happened.*/
  return 0;
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

/* Returns whether there are no more non-space characters in `s`. */
int is_end(char *s) {
  skip_spaces(&s);
  return *s == 0;
}

int parse_string(char *s, assembler_t *assembler) {
  char *last_quotes;

  if (!accept(&s, '"')) {
    /* .string directive must contain a string enclosed in quotes. */
    return 0;
  }

  last_quotes = strrchr(s, '"');
  if (last_quotes == NULL) {
    /* The string must be enclosed by two quotes. */
    return 0;
  }
  if (!is_end(last_quotes + 1)) {
    /* Extraneous text after string. */
    return 0;
  }

  while (s < last_quotes) {
    add_data(assembler, *s);
    s++;
  }

  return 1;
}

directive_kind_t get_directive_type(char *token) {
  if (strcmp(token, DIRECTIVE_DATA) == 0)
    return DIRECTIVE_KIND_DATA;
  if (strcmp(token, DIRECTIVE_STRING) == 0)
    return DIRECTIVE_KIND_STRING;
  if (strcmp(token, DIRECTIVE_MAT) == 0)
    return DIRECTIVE_KIND_MAT;
  if (strcmp(token, DIRECTIVE_ENTRY) == 0)
    return DIRECTIVE_KIND_ENTRY;
  if (strcmp(token, DIRECTIVE_EXTERN) == 0)
    return DIRECTIVE_KIND_EXTERN;
  return DIRECTIVE_KIND_UNKNOWN;
}

void compile_assembly_code(char *line, assembler_t *assembler) {
  char *temp;
  int label_flag = 0;

  temp = strtok(line, " \t");

  if (temp == NULL)
    fprintf(stderr, "How did we get here?");

  else if (*temp == '\n')
    /*Entire line of whitespace, ignore.*/
    return;

  else if (*temp == ';')
    /*We have a comment, ignore.*/
    return;

  /*From here, we can 100% be sure we either have an instruction command, or
   * data command.*/

  /*Do we have a label?*/
  int is_label_flag = is_label(temp);

  if (is_label_flag) {

    if (is_label_valid(temp, assembler) != 1)
      fprintf(stderr, "Label %s cannot be used at line %d", temp, 15);
    // else
    temp = strtok(NULL, " \t");
  }
  /*Is my token an instruction, or data label?*/
  /*If there's a dot, it means its a data command.*/
  if (*temp == '.') {
    /*NOTE: Get the data type that's in there :)*/
    char *rest = strtok(NULL, "");

    directive_kind_t type = get_directive_type(temp);
    if (type == DIRECTIVE_KIND_DATA) {
      parse_data(rest, assembler);
    }
    else if (type == DIRECTIVE_KIND_STRING) {
      parse_string(rest, assembler);
    }
    else if (type == DIRECTIVE_KIND_UNKNOWN) {
      fprintf(stderr, "Unknown datatype.");
    }

    // count_datatype_storage();

    // } else if (is_assembly_command(temp)) {
    //
    // } else {
    //   fprintf(stderr, "Invalid command");
  }
}
