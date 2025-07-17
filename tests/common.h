#ifndef TESTS_COMMON
#define TESTS_COMMON

#include <stdio.h>
#include <stdlib.h>
#include "../include/context.h"
#include "../include/codegen.h"
#include "../include/data.h"
#include "../include/list.h"
#include "../include/parser.h"
#include "../include/preprocessor.h"
#include "../include/table.h"

#define ASSERTM(a, message)                                                                                            \
  if (!(a)) {                                                                                                          \
    printf("%s:%d: %s\n", __FILE__, __LINE__, message);                                                                \
    exit(1);                                                                                                           \
  }

#define ASSERT(a) ASSERTM(a, "ASSERTION FAILED")

#endif
