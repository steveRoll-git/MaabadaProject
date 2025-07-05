
#ifndef PARSER_H
#define PARSER_H

#include "../common/data.h"
#include "../datatypes/assembler.h"
#include "../firstpass/parser.h"
typedef enum operand_kind_t {
  OPERAND_KIND_INVALID,
  OPERAND_KIND_REGISTER,
  OPERAND_KIND_WHOLE_NUMBER,
  OPERAND_KIND_LABEL,
  OPERAND_KIND_MATRIX
} operand_kind_t;

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
int parse_int(char **s, int *result);

int parse_data(char *s, assembler_t *assembler);

directive_kind_t read_directive_kind(char **s);

int compile_assembly_code(char *line, assembler_t *assembler);

operand_kind_t parse_instruction_argument(char **s, assembler_t *assembler);

int parse_instruction_args(char **s, const args_t args, assembler_t *assembler);

int accept(char **s, char c);
int is_label_valid(char *label, assembler_t *assembler);

int accept(char **s, char c);
int parse_int(char **s, int *result);
int parse_matrix_operand(char **s);
int get_word_size(operand_kind_t arg1, operand_kind_t arg2);
int parse_string(char *s, assembler_t *assembler);
int parse_matrix(char *s, assembler_t *assembler);

#endif
