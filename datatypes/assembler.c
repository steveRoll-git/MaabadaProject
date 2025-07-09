#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "table.h"

/* Represents a reference to a label. */
typedef struct label_reference_t {
  /* The index in the code image where the label's address should be written to. */
  size_t location;

  /* The line in the source code where this label was referenced, for use in error messages. */
  int line;
} label_reference_t;

/* Information about a label. */
typedef struct label_info_t {
  /* Whether this label's definition has been found. */
  bool_t found;

  /* Stores the IC value this label points to, or if this is a data label, the DC value it points to. */
  /* This is only defined after the label's definition has been found in the code. */
  size_t value;

  /* False if this label points to an instruction; True if the label points to data. */
  bool_t is_data;

  /* Whether this label was defined with a `.extern` directive. */
  bool_t is_external;

  /* List of `label_reference_t` values - references to this label. */
  list_t *references;
} label_info_t;

typedef struct assembler_t {
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

assembler_t *assembler_create() {
  assembler_t *assembler = malloc(sizeof(assembler_t));
  assembler->ic = 100;
  assembler->dc = 0;
  assembler->code_array = list_create(sizeof(machine_word_t));
  assembler->data_array = list_create(sizeof(machine_word_t));
  assembler->macro_table = table_create(sizeof(long));
  assembler->label_table = table_create(sizeof(label_info_t));
  return assembler;
}

void add_code_word(assembler_t *assembler, machine_word_t data) {
  LIST_ADD(assembler->code_array, data);
  assembler->ic++;
}

void add_data_word(assembler_t *assembler, machine_word_t data) {
  LIST_ADD(assembler->data_array, data);
  assembler->dc++;
}

/* Initializes a new empty `label_info_t` value for the given label. */
label_info_t *init_label_info(assembler_t *assembler, char *label) {
  label_info_t *info = table_add(assembler->label_table, label);
  info->references = list_create(sizeof(label_reference_t));
  return info;
}

void add_label_reference(assembler_t *assembler, char *label, int line) {
  label_info_t *info = table_get(assembler->label_table, label);
  if (info == NULL) {
    info = init_label_info(assembler, label);
  }
  LIST_ADD(info->references, ((label_reference_t) {assembler->ic, line}))
  add_code_word(assembler, 0);
}

result_t add_label(assembler_t *assembler, char *label, bool_t is_data, bool_t is_external) {
  label_info_t *info = table_get(assembler->label_table, label);

  if (info == NULL) {
    info = init_label_info(assembler, label);
  }
  else {
    /* Make sure that the label wasn't already defined. */
    ASSERT(!info->found, ERR_LABEL_ALREADY_DEFINED)
  }

  info->found = TRUE;
  info->is_data = is_data;
  info->is_external = is_external;
  if (!is_external) {
    info->value = is_data ? assembler->dc : assembler->ic;
  }

  return SUCCESS;
}

void merge_data(assembler_t *assembler) {
  int i;

  for (i = 0; i < table_count(assembler->label_table); i++) {
    label_info_t *info = table_value_at(assembler->label_table, i);
    if (info->found && info->is_data) {
      info->value += assembler->ic;
    }
  }
}

void print_data(assembler_t *assembler) {
  int i = 0;
  printf("IC: %d \n", assembler->ic);
  printf("DC: %d \n", assembler->dc);

  printf("\nARGS (For DC): { ");

  for (i = 0; i < list_count(assembler->data_array); i++) {
    printf("%d,  ", *(machine_word_t *) list_at(assembler->data_array, i));
  }
  printf("}\n");

  printf("Labels:\n");

  for (i = 0; i < table_count(assembler->label_table); i++) {
    int j;
    label_info_t *info = table_value_at(assembler->label_table, i);
    printf("%s: ", table_key_at(assembler->label_table, i));
    for (j = 0; j < list_count(info->references); j++) {
      printf("%d, ", ((label_reference_t *) list_at(info->references, j))->line);
    }
    printf("\n");
  }
}

void assembler_free(assembler_t *assembler) {
  list_free(assembler->code_array);
  list_free(assembler->data_array);
  table_free(assembler->label_table);
  free(assembler);
}
