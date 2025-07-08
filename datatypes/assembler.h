#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "linked_list.h"

/* Represents a reference to a label. */
typedef struct label_reference_t {
  /* The index in the code image where the label's address should be written to. */
  size_t location;
  /* The line in the source code where this label was referenced, for use in error messages. */
  int line;
} label_reference_t;

/* Represents an assembler as it's traversing a source file. */
typedef struct assembler_t assembler_t;

/* Creates a new assembler. */
assembler_t *assembler_create();

/* Adds a single word to the assembler's code image. */
void add_code_word(assembler_t *assembler, machine_word_t data);

/* Adds a single word to the assembler's data image. */
void add_data_word(assembler_t *assembler, machine_word_t data);

/* Adds a label that points to the current IC value. */
void add_code_label(assembler_t *assembler, char *label);

/* Adds a label that points to the current DC value. */
void add_data_label(assembler_t *assembler, char *label);

void print_data(assembler_t *assembler);

void merge_data(assembler_t *assembler);

/* Frees all the data used by the assembler, including the pointer to it. */
void assembler_free(assembler_t *assembler);
#endif
