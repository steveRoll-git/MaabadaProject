#include <string.h>


#include "common.h"

int main(void) {
  assembler_t assembler = assembler_create();
  char *in = "../examples/wont_work/matricesTesting.am";
  int result = first_pass(in, NULL, &assembler);
  print_data(&assembler);

  ASSERT(list_get(&assembler.label_table, "MAIN") == 100);

  ASSERT(list_get(&assembler.label_table, "LOOP") == 107);
  ASSERT(list_get(&assembler.label_table, "END") == 121);
  ASSERT(list_get(&assembler.label_table, "STR") == 122);
  ASSERT(list_get(&assembler.label_table, "LENGTH") == 129);
  ASSERT(list_get(&assembler.label_table, "K") == 132);
  ASSERT(list_get(&assembler.label_table, "M1") == 133);


  int expected_args[] = {'a', 'b', 'c', 'd', 'e', 'f', '\0', 6, -9, 1, 5, 22, 1, 2, 3, 4};
  ASSERT(memcmp(expected_args, assembler.data_array.ptr, sizeof(expected_args)));

  return result == TRUE;
}
