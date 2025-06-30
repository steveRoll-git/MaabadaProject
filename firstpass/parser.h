
#ifndef PARSER_H
#define PARSER_H

#include "../common/data.h"
#include "../datatypes/growing_array.h"
#include "../datatypes/linked_list.h"

/* TODO move this into a separate "assembler" file? */
/* Represents an assembler as it's traversing a source file. */
typedef struct assembler_state_t {
  /* The Instruction Counter: The address where the next instruction's first word will be. */
  int ic;

  /* The Data Counter: The relative address where the next data directive's first word will be. */
  /* During the first pass, this counter is relative. In the second pass, references to it will be corrected based on
   * `ic`. */
  int dc;

  /* The array that stores the data image. */
  growing_array_t data_array;

  /* Stores names of macros from the previous pass, to check that no labels have the same name. */
  linked_list_t macro_table;

  /* Stores the names of labels that point to instructions. */
  linked_list_t label_table;

  /* Stores the names of labels that point to data words. */
  linked_list_t data_table;
} assembler_state_t;

/* Adds a single word to the assembler's data image. */
void add_data(assembler_state_t *assembler, int data);

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s);

/* Parses an integer (with an optional + or -) and stores it in `result`. Returns whether it was successful. */
int parse_int(char **s, int *result);

int parse_data(char **s, assembler_state_t *assembler);

datatype_t get_data_type(char *token);

void compile_assembly_code(char *line, assembler_state_t *assembler);

int parse_instruction_arguement(char **s);

int parse_instruction_args(char **s, const args_t args);

#endif
