#include "../common/linked_list.h"
#include "data.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
int parse_int(char *s)
{
  int size = strlen(s);
  char buffer[size];
  int i = 0;
  while (isspace(*(s + i))) i++;


  if (*(size + i) == '-')
  {
    buffer[0] = '-';
    i++;
  }

  if (!isdigit(*(s + i)))
    return 0;

  while (i < size && isdigit(*(s + i)))
  {
    buffer[i] = *(s + i);
    i++;
  }

  while (isspace(*(s + i)) && i < size) i++;


int count_datatype_storage(datatype_t type, char* token){

}


datatype_t get_data_type(char *token) {
  if (strcmp(token, DATATYPE_DATA))
    return DATA;
  else if (strcmp(token, DATATYPE_STRING) == 0)
    return STRING;
  else if (strcmp(token, DATATYPE_MAT) == 0)
    return MAT;
  else if (strcmp(token, DATATYPE_ENTRY) == 0)
    return ENTRY;
  else if (strcmp(token, DATATYPE_EXTERN) == 0)
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

  /*Entire line of whitespace, ignore.*/
  else if (*temp == '\n')
    return;

  /*We have a comment, ignore.*/
  else if (*temp == ';')
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
