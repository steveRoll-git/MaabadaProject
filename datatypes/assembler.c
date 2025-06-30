#include "assembler.h"

assembler_t assembler_create() {
  assembler_t assembler;
  assembler.ic = 100;
  assembler.dc = 0;
  assembler.data_array = growing_array_create();
  assembler.macro_table = list_init();
  assembler.label_table = list_init();
  assembler.data_table = list_init();
  return assembler;
}

void add_data(assembler_t *assembler, int data) {
  growing_array_add(&assembler->data_array, data);
  assembler->dc++;
}
