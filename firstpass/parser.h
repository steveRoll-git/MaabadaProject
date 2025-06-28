
#ifndef PARSER_H
#define PARSER_H
#include "../external/data.h"
#include "../datatypes/linked_list.h"

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s);

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
int parse_int(char **s, int *result);

int parse_ddata(char **s,  linked_list_t *args);


datatype_t get_data_type(char *token);

void compile_assembly_code(char *line, linked_list_t *macro_table,
                           linked_list_t *label_table,
                           linked_list_t *data_table) ;

#endif //PARSER_H
