#include "../common/linked_list.h"

void compile_line(char *line) {
}

/* TODO temp arguments */
int first_pass(char *input_file_path, char *output_file_path) {
  /*Instruction counter, and Data counter initilization.*/
  int i_counter = 100, d_counter = 0;
  linked_list_t data_table = list_init();
  linked_list_t instruction_table = list_init();
  linked_list_t args = list_init();
}
