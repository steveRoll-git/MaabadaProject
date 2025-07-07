#include "common.h"

static statement_t statement;

int main(void) {
#ifndef MAX_INTEGER
  return 0;
#endif

  // Numbers 952 and -1000 are way too big to fit inside 10bit numbers, throw an error.
  char data[] = ".data 1,5,  2 ,-22, 952, -1000";
  // There has to be an error here, since the file we've created has integers too big for its buffer.
  // it needs to throw the error number 0. if we get 1 (everything worked) then its fuked bruv;

  result_t flag = parse_statement(data, &statement);

  ASSERTM(flag == FALSE, "Numbers 952 and -1000 are way too big to fit inside 10bit numbers");
  return 0;
}
