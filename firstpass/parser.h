
#ifndef PARSER_H
#define PARSER_H

#include "../common/data.h"
#include "../datatypes/assembler.h"

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s);

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
int parse_int(char **s, int *result);

int parse_data(char **s, assembler_t *assembler);

datatype_t get_data_type(char *token);

void compile_assembly_code(char *line, assembler_t *assembler);

int parse_instruction_arguement(char **s);

int parse_instruction_args(char **s, const args_t args);

#endif
