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

int parse_data(char **s, assembler_state_t *assembler) {
  do {
    int number;
    skip_spaces(s);
    if (!parse_int(s, &number)) {
      printf("Malformed number.\n");
      return 0;
    }
    add_data(assembler, number);
    skip_spaces(s);
  }
  while (**s == ',');

  return 1;
}
/* Changes PTR, doesn't Return error codes (the way parse_int works), it's a building block for every other function.*/
int parse_instruction_arguement(char **s) {
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

  /*TODO: This algorithm needs more refining.*/
  if (is_register(*s)) {
    *s += size;
    return 1;
  }

  /* Must be label,
   * or worse, a label with a matrix addition!
   */
  /*TODO: Uncomment this line and have a chat with ron, this needs to  get a merge between our codes so it can work. we
   * need to add another arguement (The object macro table shit we get from firstpass) into this function. and WE NEED
   * this function for checking if the parsing of the label is correct.*/

  // if (!is_label_valid(*s))
  /* Skip between all the characters of the label. not important in first pass.*/
  *s += size;

  //
  // if (**s == '[') {
  //   /*  parse_matrix_values(s); TODO: Should return ERROR if the parsing of the matrix isnt correct.*/
  // }
  skip_spaces(s);
  return 1;
}

int parse_matrix_values(char **s) {
  if (**s != '[')
    return 0;

  (*s)++;

  parse_instruction_arguement(s);

  if (**s != ']')
    return 0;
  skip_spaces(s);
  return 1;
}

int parse_instruction_args(char **s, const args_t args) {
  skip_spaces(s);

  switch (args) {
    case NO_ARGS:
      return **s == '\0';


    case ONE_ARG:
      if (!parse_instruction_arguement(s))
        return 0;
      skip_spaces(s);
      return **s == '\0';

    case TWO_ARGS:
      parse_instruction_arguement(s);
      skip_spaces(s);

      if (**s != ',')
        return 0;
      (*s)++;


      skip_spaces(s);
      parse_instruction_arguement(s);
      skip_spaces(s);
      return **s == '\0';
  }
  /*IF we're here... something really bad must've happened.*/
  return 0;
}

datatype_t get_data_type(char *token) {
  if (strcmp(token, DATATYPE_DATA) == 0)
    return DATA;
  if (strcmp(token, DATATYPE_STRING) == 0)
    return STRING;
  if (strcmp(token, DATATYPE_MAT) == 0)
    return MAT;
  if (strcmp(token, DATATYPE_ENTRY) == 0)
    return ENTRY;
  if (strcmp(token, DATATYPE_EXTERN) == 0)
    return EXTERN;
  return UNKNOWN;
}

void compile_assembly_code(char *line, assembler_state_t *assembler) {
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
    /*TODO: we got an error, great, now, make it possible to let the function
     * know what to do.*/
    // if (is_label_valid(temp, arr) != 1)
    fprintf(stderr, "Label %s cannot be used at line %d", temp, 15);
    // else
    temp = strtok(NULL, " \t");
  }
  /*Is my token an instruction, or data label?*/
  /*If there's a dot, it means its a data command.*/
  if (*temp == '.') {
    /*NOTE: Get the data type that's in there :)*/

    datatype_t type = get_data_type(temp);
    if (type == UNKNOWN) {
      fprintf(stderr, "Unknown datatype.");
    }

    // count_datatype_storage();

    // } else if (is_assembly_command(temp)) {
    //
    // } else {
    //   fprintf(stderr, "Invalid command");
  }
}
