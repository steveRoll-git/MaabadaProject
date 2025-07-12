#ifndef TESTS_COMMON
#define TESTS_COMMON

#include <stdio.h>
#include <stdlib.h>
#include "../include/assembler.h"
#include "../include/codegen.h"
#include "../include/data.h"
#include "../include/list.h"
#include "../include/parser.h"
#include "../include/preprocessor.h"
#include "../include/table.h"

/* Assembler is redefined here so we can access its members easily */

/* Represents a reference to a label. */
typedef struct label_reference_t {
  /* The index in the code image where the label's address should be written to. */
  int location;

  /* The line in the source code where this label was referenced, for use in error messages. */
  int line;
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


#define ASSERTM(a, message)                                                                                            \
  if (!(a)) {                                                                                                          \
    printf("%s:%d: %s\n", __FILE__, __LINE__, message);                                                                \
    exit(1);                                                                                                           \
  }

#define ASSERT(a) ASSERTM(a, "ASSERTION FAILED")


#endif
