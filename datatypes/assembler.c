#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>

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

assembler_t *assembler_create() {
  assembler_t *assembler = malloc(sizeof(assembler_t));
  assembler->ic = 100;
  assembler->dc = 0;
  assembler->code_array = array_create(sizeof(machine_word_t));
  assembler->data_array = array_create(sizeof(machine_word_t));
  assembler->macro_table = list_init();
  assembler->label_table = list_init();
  assembler->data_table = list_init();
  return assembler;
}

void add_code_word(assembler_t *assembler, machine_word_t data) {
  ARRAY_ADD(assembler->code_array, data);
  assembler->ic++;
}

void add_data_word(assembler_t *assembler, machine_word_t data) {
  ARRAY_ADD(assembler->data_array, data);
  assembler->dc++;
}

void add_code_label(assembler_t *assembler, char *label) {
  list_add(&assembler->label_table, label, assembler->ic);
}

void add_data_label(assembler_t *assembler, char *label) {
  list_add(&assembler->data_table, label, assembler->dc);
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

  for (i = 0; i < array_count(assembler->data_array); i++) {
    printf("%d,  ", *(machine_word_t *) array_at(assembler->data_array, i));
  }
  printf("}\n");
}

void assembler_free(assembler_t *assembler) {
  array_free(assembler->code_array);
  array_free(assembler->data_array);
  purge_list(&assembler->data_table);
  purge_list(&assembler->label_table);
  free(assembler);
}
