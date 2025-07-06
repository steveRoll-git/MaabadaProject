#ifndef PARSER_H
#define PARSER_H

#include "../common/data.h"
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

/* Information about a call to an instruction. */
typedef struct instruction_t {
  /* The info for the invoked instruction. */
  instruction_info_t *info;

  /* The number of arguments the instruction was called with. */
  num_args_t num_args;

  /* The first operand (set only if `num_args` is at least 1.) */
  operand_t operand_1;

  /* The second operand (set only if `num_args` is 2.) */
  operand_t operand_2;
} instruction_t;

/* Information about a directive. */
typedef struct directive_t {
  /* What kind of directive this is. */
  directive_kind_t kind;

  /* Additional info about this directive. */
  union {
    /* If this is a .data, .string or .mat directive, this stores the data that was given. */
    struct {
      /* The contents of the data. */
      machine_word_t array[MAX_LINE];
      /* The size of the data. */
      int size;
    } data;
    /* If this is a .entry or .extern directive, this stores the name of the label that was given. */
    char label[MAX_LINE];
  } info;
} directive_t;

/* The kinds of statements that can be parsed. */
typedef enum statement_kind_t {
  /* The statement is empty, or contains a comment. */
  STATEMENT_EMPTY,
  /* The statement contains an instruction. */
  STATEMENT_INSTRUCTION,
  /* The statement contains a directive. */
  STATEMENT_DIRECTIVE
} statement_kind_t;

/* Information about a parsed statement of assembly code. */
typedef struct statement_t {
  /* What kind of statement this is. */
  statement_kind_t kind;

  /* Whether this line has an attached statement. */
  bool_t has_label;

  /* The statement's label name. */
  char label[MAX_LABEL];

  /* Stores the data for the instruction or directive. */
  union {
    /* If `kind` is `STATEMENT_INSTRUCTION`, stores information about the instruction. */
    instruction_t instruction;
    /* If `kind` is `STATEMENT_DIRECTIVE`, stores information about the directive. */
    directive_t directive;
  } data;
} statement_t;

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
bool_t parse_number(char **s, machine_word_t *result);

/* Parses between 0 and 2 operands of an instruction. */
bool_t parse_instruction_operands(char *s, instruction_t *instruction);

bool_t parse_data(char *s, directive_t *directive);

directive_kind_t read_directive_kind(char **s);

bool_t parse_statement(char *line, statement_t *statement);

bool_t parse_operand(char **s, operand_t *operand);

bool_t accept(char **s, char c);
bool_t parse_matrix_operand(char **s, operand_t *operand);
int get_word_size(operand_kind_t arg1, operand_kind_t arg2);
bool_t parse_string(char *s, directive_t *directive);
bool_t parse_matrix(char *s, directive_t *directive);

#endif
