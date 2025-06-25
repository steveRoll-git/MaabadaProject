#include "../common/linked_list.h"
#include "data.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

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

datatype_t get_data_type(char *token) {
  if (strcmp(token, DATATYPE_DATA))
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

void compile_assembly_code(char *line, linked_list_t *macro_table,
                           linked_list_t *label_table,
                           linked_list_t *data_table) {
  char *temp;
  int label_flag = 0;

  linked_list_t *arr[] = {macro_table, label_table, data_table};

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
