
#ifndef PARSER_H
#define PARSER_H

#include "../common/data.h"
#include "../datatypes/assembler.h"
#include "../firstpass/parser.h"
typedef enum operand_kind {
  OPERAND_KIND_INVALID,
  OPERAND_KIND_REGISTER,
  OPERAND_KIND_WHOLE_NUMBER,
  OPERAND_KIND_LABEL,
  OPERAND_KIND_MATRIX
} operand_kind_t;

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s);

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
int parse_int(char **s, int *result);

int parse_data(char *s, assembler_t *assembler);

directive_kind_t get_directive_kind(char *token);

int compile_assembly_code(char *line, assembler_t *assembler);

operand_kind_t parse_instruction_argument(char **s, assembler_t *assembler);

int parse_instruction_args(char **s, const args_t args, assembler_t *assembler);

#endif
