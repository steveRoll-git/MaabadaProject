#ifndef TESTS_COMMON
#define TESTS_COMMON
#include <stdio.h>
#include <stdlib.h>
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "../firstpass/firstpass.h"
#include "../firstpass/parser.h"
#include "../preprocess/macroprocessor.h"

/* Assembler is redefined here so we can access its members easily */

typedef struct assembler_t {
  /* The Instruction Counter: The address where the next instruction's first word will be. */
  int ic;

  /* The Data Counter: The relative address where the next data directive's first word will be. */
  /* During the first pass, this counter is relative. In the second pass, references to it will be corrected based on
   * `ic`. */
  int dc;

  /* The array that stores the code image. */
  array_t *code_array;

  /* The array that stores the data image. */
  array_t *data_array;

  /* Stores names of macros from the previous pass, to check that no labels have the same name. */
  linked_list_t macro_table;

  /* Stores the names of labels that point to instructions. */
  linked_list_t label_table;

  /* Stores the names of labels that point to data words. */
  linked_list_t data_table;
} assembler_t;

#define ASSERTM(a, message)                                                                                            \
  if (!(a)) {                                                                                                          \
    printf("%s:%d: %s\n", __FILE__, __LINE__, message);                                                                \
    exit(1);                                                                                                           \
  }

#define ASSERT(a) ASSERTM(a, "ASSERTION FAILED")


#endif
