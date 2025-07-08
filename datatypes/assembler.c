#include "assembler.h"

#include <stdio.h>

assembler_t assembler_create() {
  assembler_t assembler;
  assembler.ic = 100;
  assembler.dc = 0;
  assembler.code_array = array_create(sizeof(machine_word_t));
  assembler.data_array = array_create(sizeof(machine_word_t));
  assembler.macro_table = list_init();
  assembler.label_table = list_init();
  assembler.data_table = list_init();
  return assembler;
}

void add_code_word(assembler_t *assembler, machine_word_t data) {
  ARRAY_ADD(&assembler->code_array, data);
  assembler->ic++;
}

void add_data_word(assembler_t *assembler, machine_word_t data) {
  ARRAY_ADD(&assembler->data_array, data);
  assembler->dc++;
}

void merge_data(assembler_t *assembler) {
  list_node_t *node = assembler->data_table.head;


  while (node != NULL) {
    list_add(&assembler->label_table, node->name, node->value + assembler->ic);
    node = node->next;
  }

  purge_list(&assembler->data_table);
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
    printf("%d,  ", ((machine_word_t *) assembler->data_array.ptr)[i]);
  }
  printf("}\n");
}
