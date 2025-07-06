
#ifndef PARSER_H
#define PARSER_H

#include "../common/data.h"
#include "../datatypes/assembler.h"
#include "../firstpass/parser.h"

/* The different kinds of operands. */
typedef enum operand_kind_t {
  OPERAND_KIND_IMMEDIATE,
  OPERAND_KIND_LABEL,
  OPERAND_KIND_MATRIX,
  OPERAND_KIND_REGISTER,
} operand_kind_t;

/* Information about an operand in an instruction. */
typedef struct operand_t {
  /* What kind of operand this is. */
  operand_kind_t kind;

  /* Stores the operand's data, with different data depending on what `kind` it is. */
  union {
    /* If `kind` is `OPERAND_KIND_IMMEDIATE`, stores the value of the immediate number. */
    int immediate;

    /* If `kind` is `OPERAND_KIND_LABEL`, stores the label's name. */
    char label[MAX_LABEL];

    /* If `kind` is `OPERAND_KIND_REGISTER`, stores the register's index, from 0 to 7. */
    char register_index;

    /* If `kind` is `OPERAND_KIND_MATRIX`, stores info about how the matrix is accessed. */
    struct {
      /* The label of the matrix being accessed. */
      char label[MAX_LABEL];
      /* The index of the register used to index the row. */
      char row_reg;
      /* The index of the register used to index the column. */
      char col_reg;
    } matrix;
  } data;
} operand_t;

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
bool_t parse_int(char **s, int *result);

bool_t parse_data(char *s, assembler_t *assembler);

directive_kind_t read_directive_kind(char **s);

bool_t compile_assembly_code(char *line, assembler_t *assembler);

bool_t parse_operand(char **s, operand_t *operand);

bool_t parse_instruction_args(char **s, const num_args_t args, assembler_t *assembler);

bool_t accept(char **s, char c);
int is_label_valid(char *label, assembler_t *assembler);

bool_t accept(char **s, char c);
bool_t parse_int(char **s, int *result);
bool_t parse_matrix_operand(char **s, operand_t *operand);
int get_word_size(operand_kind_t arg1, operand_kind_t arg2);
bool_t parse_string(char *s, assembler_t *assembler);
bool_t parse_matrix(char *s, assembler_t *assembler);

#endif
