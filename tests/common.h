#ifndef TESTS_COMMON
#define TESTS_COMMON

#include <stdlib.h>
#include "../common/data.h"
#include "../common/utils.h"
#include "../datatypes/assembler.h"
#include "../datatypes/linked_list.h"
#include "../firstpass/firstpass.h"
#include "../firstpass/parser.h"
#include "../preprocess/macroprocessor.h"

#define ASSERTM(a, message)                                                                                            \
  if (!(a)) {                                                                                                          \
    printf("%s:%d: %s\n", __FILE__, __LINE__, message);                                                                \
    exit(1);                                                                                                           \
  }

#define ASSERT(a) ASSERTM(a, "ASSERTION FAILED")


#endif
