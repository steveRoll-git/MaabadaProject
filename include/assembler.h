#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../include/errors.h"
#include "data.h"
#include "list.h"
#include "table.h"

/* The address where the code image starts in memory. */
/* This is the initial value of the assembler's IC. */
#define CODE_IMAGE_START_ADDRESS 100

/* Represents a reference to a label. */
typedef struct label_reference_t {
  /* The index in the code image where the label's address should be written to. */
  int location;

  /* The line number in the source code where this label was referenced, for use in error messages. */
  int line_number;
} label_reference_t;

/* Information about a label. */
typedef struct label_info_t {
  /* Whether this label's definition has been found. */
  bool_t found;

  /* Stores the IC value this label points to, or if this is a data label, the DC value it points to. */
  /* This is only defined after the label's definition has been found in the code. */
  int value;

  /* False if this label points to an instruction; True if the label points to data. */
  bool_t is_data;

  /* Whether this label was defined with a `.extern` directive. */
  bool_t is_external;

  /* Whether this label was specified as an entry using the `.entry` directive. */
  bool_t is_entry;

  /* If `is_entry` is true, this stores the line_number number at which the `.entry` directive appeared. */
  int entry_line;

  /* List of `label_reference_t` values - references to this label. */
  list_t *references;
} label_info_t;

/* Represents an assembler as it's traversing a source file. */
typedef struct assembler_t {
  /* The path to the ".am" file being assembled. Used in error messages. */
  char *file_path;

  /* The number of the line currently being processed. */
  int line_number;

  /* The Instruction Counter: The address where the next instruction's first word will be. */
  int ic;

  /* The Data Counter: The relative address where the next data directive's first word will be. */
  /* During the first pass, this counter is relative. In the second pass, references to it will be corrected based on
   * `ic`. */
  int dc;

  /* List of `machine_word_t` - stores the code image. */
  list_t *code_array;

  /* List of `machine_word_t` - stores the data image. */
  list_t *data_array;

  /* Stores names of macros from the previous pass, to check that no labels have the same name. */
  table_t *macro_table;

  /* Associates label names with `label_info_t` values. */
  table_t *label_table;
} assembler_t;

/* Creates a new assembler. Requires the macro table from the preprocessing stage. */
assembler_t *assembler_create(char *file_path, table_t *macro_table);

/* Adds a single word to the assembler's code image. */
void add_code_word(assembler_t *assembler, machine_word_t data);

/* Adds a single word to the assembler's data image. */
void add_data_word(assembler_t *assembler, machine_word_t data);

/* Adds a single word to the code image, that will reference a label after it's resolved. */
void add_label_reference(assembler_t *assembler, char *label);

/* Adds a label to the label table, and sets its definition based on the current values of IC/DC. */
result_t add_label(assembler_t *assembler, char *label, bool_t is_data);

/* Specifies that the given label is an entry. */
result_t add_entry(assembler_t *assembler, char *label);

/* Specifies that the given label is external. */
result_t add_extern(assembler_t *assembler, char *label);

/* Called after the whole file's code has been generated. */
/* Updates the location of all data labels, so that they will point to the correct region after the code image. */
void merge_data(assembler_t *assembler);

/* Resolves all label references. All label operands in the code image will have the label's value, if it's defined. */
/* If there are any references to labels that were not defined, prints error messages for them and returns FALSE,
 * otherwise returns TRUE. */
bool_t resolve_labels(assembler_t *assembler);

/* Prints the assembler's state, for debugging purposes. */
void print_data(assembler_t *assembler);

/* Frees all the data used by the assembler, including the pointer to it. */
void assembler_free(assembler_t *assembler);
#endif
