#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "data.h"
#include "errors.h"

/* The different kinds of words that can be read by `read_word`. */
typedef enum word_kind_t {
  /* The text that was attempted to be read is not a word. */
  WORD_NONE,
  /* The 'mcro' preprocessor keyword. */
  WORD_MCRO,
  /* The 'mcroend' preprocessor keyword. */
  WORD_MCROEND,
  /* The name of a directive. */
  WORD_DIRECTIVE,
  /* The name of an instruction. */
  WORD_INSTRUCTION,
  /* A name of a register: 'r0' through 'r7'. */
  WORD_REGISTER,
  /* Any name that isn't a reserved word. */
  WORD_IDENTIFIER
} word_kind_t;

/* A sequence of alphanumeric characters that may represent a keyword in the assembler. */
typedef struct word_t {
  /* What kind of word this is. */
  word_kind_t kind;

  /* This word's contents. */
  char value[MAX_LINE];

  /* If `kind` is `WORD_REGISTER`, this stores the register's index, from 0 to 7. */
  char register_index;

  /* If `kind` is `WORD_DIRECTIVE`, this stores what directive this word is. */
  directive_kind_t directive_kind;

  /* If `kind` is `WORD_INSTRUCTION`, this stores a pointer to the instruction. */
  instruction_info_t *instruction_info;
} word_t;

/* The different kinds of operands. */
typedef enum operand_kind_t {
  OPERAND_KIND_IMMEDIATE,
  OPERAND_KIND_LABEL,
  OPERAND_KIND_MATRIX,
  OPERAND_KIND_REGISTER
} operand_kind_t;

/* Information about an operand in an instruction. */
typedef struct operand_t {
  /* What kind of operand this is. */
  operand_kind_t kind;

  /* Stores the operand's data, with different data depending on what `kind` it is. */
  union {
    /* If `kind` is `OPERAND_KIND_IMMEDIATE`, stores the value of the immediate number. */
    machine_word_t immediate;

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
      machine_word_t array[DIRECTIVE_MAX_DATA];

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

/* The possible results that `read_line` can return. */
typedef enum read_line_status_t {
  READ_LINE_SUCCESS,
  READ_LINE_TOO_LONG,
  READ_LINE_EOF
} read_line_status_t;

/**
 * Returns whether there are no more non-space characters in `s`.
 *
 * @param s The string to check.
 * @return Whether the rest of `s` is only whitespace characters.
 */
bool_t is_end(char *s);

/**
 * Reads a single line from the file that is at most `MAX_LINE` bytes long, and stores it in `line`. The line may end
 * with a newline or a null terminator.
 *
 * @param file The file to read a line from.
 * @param line The array to store the line into. The array must be at least 81 bytes long.
 * @return `READ_LINE_SUCCESS` if a line was read successfully, `READ_LINE_TOO_LONG` if the line was too long, and
 * `READ_LINE_EOF` if there are no more lines to read.
 */
read_line_status_t read_line(FILE *file, char line[MAX_LINE]);

/**
 * Reads the next word (a sequence of alphanumeric characters that starts with a letter) at the string pointed to by
 * `s`.
 * Updates `s` so that it will point to the next character after the word that was read.
 *
 * @param s A pointer to the string to read the next word from. Will be updated to point to the next character, if a
 * word was read.
 * @param word A pointer to the word object where info about it will be stored.
 */
void read_word(char **s, word_t *word);

/**
 * Parses an assembly statement and stores it in the given pointer.
 * May fail if the statement contains syntax errors.
 *
 * @param line The string containing the statement to parse.
 * @param statement A pointer to the statement object where info about it will be stored.
 * @return The operation's result.
 */
result_t parse_statement(char *line, statement_t *statement);

#endif
