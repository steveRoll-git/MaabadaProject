#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "array.h"
#include "linked_list.h"

/* Represents an assembler as it's traversing a source file. */
typedef struct assembler_t {
  /* The Instruction Counter: The address where the next instruction's first word will be. */
  int ic;

  /* The Data Counter: The relative address where the next data directive's first word will be. */
  /* During the first pass, this counter is relative. In the second pass, references to it will be corrected based on
   * `ic`. */
  int dc;

  /* The array that stores the code image. */
  array_t code_array;

  /* The array that stores the data image. */
  array_t data_array;

  /* Stores names of macros from the previous pass, to check that no labels have the same name. */
  linked_list_t macro_table;

  /* Stores the names of labels that point to instructions. */
  linked_list_t label_table;

  /* Stores the names of labels that point to data words. */
  linked_list_t data_table;
} assembler_t;

/* Creates a new assembler. */
assembler_t assembler_create();

/* Adds a single word to the assembler's code image. */
void add_code_word(assembler_t *assembler, machine_word_t data);

/* Adds a single word to the assembler's data image. */
void add_data_word(assembler_t *assembler, machine_word_t data);

void print_data(assembler_t *assembler);

void merge_data(assembler_t *assembler);
#endif
