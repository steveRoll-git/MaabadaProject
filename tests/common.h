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

/* Assembler's other types are redefined here so we can access their members easily */

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

#define ASSERTM(a, message)                                                                                            \
  if (!(a)) {                                                                                                          \
    printf("%s:%d: %s\n", __FILE__, __LINE__, message);                                                                \
    exit(1);                                                                                                           \
  }

#define ASSERT(a) ASSERTM(a, "ASSERTION FAILED")

#endif
