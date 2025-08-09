/*
 * context.h
 *
 * This file defines the assembler context - an object that stores all the state that is needed as a ".am" file is being
 * processed. The most important parts are the code and data images, and the label table. But other information is
 * stored as well, such as the name of the file being processed, and the number of the line currently being processed.
 *
 * All methods in this file perform various actions on the context, which are used mostly by the codegen stage.
 * These actions include: adding words to the code/data images, adding definitions and references to labels, and some
 * actions that are run after the codegen stage, like correcting the addresses of data labels and resolving label references.
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "data.h"
#include "errors.h"
#include "list.h"
#include "table.h"

/* The address where the code image starts in memory. */
/* This is the initial value of the assembler's IC. */
#define CODE_IMAGE_START_ADDRESS 100

/* Represents a reference to a label. */
typedef struct label_reference_t {
  /* The address in the code image where the label's address should be written to. */
  int address;

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

/* Represents the assembler's context - the state of the assembler as it's traversing a source file. */
typedef struct context_t {
  /* The path to the ".am" file being assembled. Used in error messages. */
  char *file_path;

  /* The number of the line currently being processed. */
  int line_number;

  /* The Instruction Counter: The address where the next instruction's first word will be. */
  int ic;

  /* The Data Counter: The relative address where the next data directive's first word will be. */
  /* This index is relative to the beginning of the data image, not a machine address. */
  int dc;

  /* List of `machine_word_t` - stores the code image. */
  list_t *code_array;

  /* List of `machine_word_t` - stores the data image. */
  list_t *data_array;

  /* Stores names of macros from the previous pass, to check that no labels have the same name. */
  table_t *macro_table;

  /* Associates label names with `label_info_t` values. */
  table_t *label_table;

  /* Whether we already warned the user about the program being too large. (That error is only shown once.) */
  bool_t warned_too_large;
} context_t;

/**
 * Creates a new assembler context. Requires the macro table from the preprocessing stage.
 * May fail if memory allocations did not succeed.
 *
 * @param file_path The file path to the .am file that will be assembled.
 * @param macro_table The macro table from the preprocess stage.
 * @param context A pointer to the pointer where the context will be stored.
 * @return The operation's result.
 */
result_t context_create(char *file_path, table_t *macro_table, context_t **context);

/**
 * Adds a single word to the context's code image.
 * May fail if:
 * - The added word causes the program to be larger than can be addressed in code.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @param word The code word to add.
 * @return The operation's result.
 */
result_t add_code_word(context_t *context, machine_word_t word);

/**
 * Adds a single word to the context's data image.
 * May fail if:
 * - The added word causes the program to be larger than can be addressed in code.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @param word The word of data to add.
 * @return The operation's result.
 */
result_t add_data_word(context_t *context, machine_word_t word);

/**
 * Adds a single word to the code image, that will reference a label after it's resolved.
 * May fail if:
 * - The added word causes the program to be larger than can be addressed in code.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @param label The name of the label being referenced.
 * @return The operation's result.
 */
result_t add_label_reference(context_t *context, char *label);

/**
 * Adds a label to the label table, and sets its definition based on the current values of IC/DC.
 * May fail if:
 * - A label with the same name already exists in the label table.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @param label The label's name.
 * @param is_data True if the label points to the data image, false if it points to the code image.
 * @return The operation's result.
 */
result_t add_label(context_t *context, char *label, bool_t is_data);

/**
 * Specifies that the given label is an entry.
 * May fail if:
 * - The label was already specified as an entry.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @param label The label's name.
 * @return The operation's result.
 */
result_t add_entry(context_t *context, char *label);

/**
 * Specifies that the given label is external.
 * May fail if:
 * - The label was already defined somewhere else (either with label syntax or another `.extern` directive).
 * - Memory allocations did not succeed.
 *
 * @param context The context to operate on.
 * @param label The label's name.
 * @return The operation's result.
 */
result_t add_extern(context_t *context, char *label);

/**
 * Called after the whole file's code has been generated.
 * Updates the location of all data labels, so that they will point to the correct region after the code image.
 *
 * @param context The assembler context to operate on.
 */
void merge_data(context_t *context);

/**
 * Resolves all label references. All label operands in the code image will have the label's value, if it's defined.
 * May fail if there are references to labels that weren't defined.
 *
 * @param context The assembler context to operate on.
 * @return The operation's result.
 */
result_t resolve_labels(context_t *context);

/**
 * Frees all the data used by the context, including the pointer to it.
 * If the given pointer is NULL, nothing is performed.
 *
 * @param context The assembler context to free. May be NULL.
 */
void context_free(context_t *context);
#endif
