#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../common/data.h"
#include "../common/errors.h"

/* Represents an assembler as it's traversing a source file. */
typedef struct assembler_t assembler_t;

/* Creates a new assembler. */
assembler_t *assembler_create();

/* Adds a single word to the assembler's code image. */
void add_code_word(assembler_t *assembler, machine_word_t data);

/* Adds a single word to the assembler's data image. */
void add_data_word(assembler_t *assembler, machine_word_t data);

/* Adds a label to the label table, and sets its definition based on the current values of IC/DC. */
result_t add_label(assembler_t *assembler, char *label, bool_t is_data, bool_t is_external);

void print_data(assembler_t *assembler);

void merge_data(assembler_t *assembler);

/* Frees all the data used by the assembler, including the pointer to it. */
void assembler_free(assembler_t *assembler);
#endif
