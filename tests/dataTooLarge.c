#include <string.h>

#include "common.h"

result_t compile_statement(context_t *context, statement_t *statement);

int main() {
  statement_t statement;
  context_t *context;

  context_create("", NULL, &context);

  ASSERT(parse_statement(".mat [100][100] 1, 2, 3", &statement) == SUCCESS);
  ASSERT(statement.kind == STATEMENT_DIRECTIVE)
  ASSERT(statement.data.directive.kind == DIRECTIVE_KIND_MAT)
  ASSERT(statement.data.directive.info.data.size == 10000)
  machine_word_t expected[DIRECTIVE_MAX_DATA] = {1, 2, 3, 0};
  ASSERT(memcmp(statement.data.directive.info.data.array, expected, DIRECTIVE_MAX_DATA) == 0)

  ASSERT(compile_statement(context, &statement) == ERR_PROGRAM_TOO_LARGE)

  return EXIT_SUCCESS;
}
