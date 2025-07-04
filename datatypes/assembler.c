#include "assembler.h"

#include <stdio.h>

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

void print_data(assembler_t *assembler) {
  int i = 0;
  printf("IC: %d \n", assembler->ic);
  printf("DC: %d \n", assembler->dc);

  printf("Label Table: ");
  print_list(&assembler->label_table);
  printf("\n Data Table: ");
  print_list(&assembler->data_table);

  printf("\n ARGS (For DC): { ");

  for (i = 0; i < assembler->data_array.count; i++) {
    printf("%d,  ", assembler->data_array.ptr[i]);
  }
  printf("}\n");
}
