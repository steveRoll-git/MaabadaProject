#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "../include/errors.h"
#include "data.h"
#include "table.h"

/* Represents an assembler as it's traversing a source file. */
typedef struct assembler_t assembler_t;

/* Creates a new assembler. Requires the macro table from the preprocessing stage. */
assembler_t *assembler_create(table_t *macro_table);

/* Adds a single word to the assembler's code image. */
void add_code_word(assembler_t *assembler, machine_word_t data);

/* Adds a single word to the assembler's data image. */
void add_data_word(assembler_t *assembler, machine_word_t data);

/* Adds a single word to the code image, that will reference a label after it's resolved. */
void add_label_reference(assembler_t *assembler, char *label, int line_number);

/* Adds a label to the label table, and sets its definition based on the current values of IC/DC. */
result_t add_label(assembler_t *assembler, char *label, bool_t is_data, bool_t is_external);

/* Specifies that the given label is an entry. */
result_t add_entry(assembler_t *assembler, char *label, int line_number);

/* Specifies that the given label is external. */
result_t add_extern(assembler_t *assembler, char *label);

/* Called after the whole file's code has been generated. */
/* Updates the location of all data labels, so that they will point to the correct region after the code image. */
void merge_data(assembler_t *assembler);

/* Resolves all label references. All label operands in the code image will have the label's value, if it's defined. */
/* If there are any references to labels that were not defined, prints error messages for them and returns FALSE,
 * otherwise returns TRUE. */
bool_t resolve_labels(assembler_t *assembler, char *file_path);

/* Prints the assembler's state, for debugging purposes. */
void print_data(assembler_t *assembler);

/* Frees all the data used by the assembler, including the pointer to it. */
void assembler_free(assembler_t *assembler);
#endif
