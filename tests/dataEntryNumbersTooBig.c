#include "common.h"

int main(void) {
#ifndef MAX_INTEGER
  return 0;
#endif

  // Numbers 952 and -1000 are way too big to fit inside 10bit numbers, throw an error.
  char data[] = ".data 1,5,  2 ,-22, 952, -1000";
  assembler_t assembler = assembler_create();
  // There has to be an error here, since the file we've created has integers too big for its buffer.
  // it needs to throw the error number 0. if we get 1 (everything worked) then its fuked bruv;
  bool_t flag = compile_assembly_code(data, &assembler);

  ASSERTM(flag == FALSE, "Numbers 952 and -1000 are way too big to fit inside 10bit numbers");
  return 0;
}
