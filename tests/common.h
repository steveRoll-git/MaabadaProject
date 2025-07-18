#ifndef TESTS_COMMON
#define TESTS_COMMON

#include <stdio.h>
#include <stdlib.h>
#include "../headers/codegen.h"
#include "../headers/context.h"
#include "../headers/data.h"
#include "../headers/list.h"
#include "../headers/parser.h"
#include "../headers/preprocessor.h"
#include "../headers/table.h"

#define ASSERTM(a, message)                                                                                            \
  if (!(a)) {                                                                                                          \
    printf("%s:%d: %s\n", __FILE__, __LINE__, message);                                                                \
    exit(1);                                                                                                           \
  }

#define ASSERT(a) ASSERTM(a, "ASSERTION FAILED")

#endif
