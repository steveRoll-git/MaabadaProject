#include "common.h"

#define ARRAY_SIZE(arr, type) (sizeof(arr) / sizeof(type))

#define ARR(...) __VA_ARGS__

static assembler_t assembler;
static int expected_dc = 0;

int check_data_equals(int *expected_data, int size) {
  int i;
  for (i = 0; i < size; i++) {
    if (assembler.data_array.ptr[assembler.dc - size + i] != expected_data[i]) {
      return FALSE;
    }
  }
  expected_dc += size;
  if (assembler.dc != expected_dc) {
    return FALSE;
  }
  return TRUE;
}

#define ASSERT_DATA_EQUALS(expected_arr)                                                                               \
  do {                                                                                                                 \
    int expected_data[] = expected_arr;                                                                                \
    ASSERT(check_data_equals(expected_data, ARRAY_SIZE(expected_data, int)))                                           \
  }                                                                                                                    \
  while (0);

int main(void) {
  assembler = assembler_create();

  ASSERT(compile_assembly_code(".data 1, 2, 9, 4", &assembler) == TRUE)
  ASSERT_DATA_EQUALS(ARR({1, 2, 9, 4}))

  ASSERT(compile_assembly_code("somelabel: .mat[2][3] 5, 2, 2  , 6, 23, 5    ", &assembler) == TRUE)
  ASSERT_DATA_EQUALS(ARR({5, 2, 2, 6, 23, 5}))

  ASSERT(compile_assembly_code("   .mat   [ 1   ] [ 1 ] -65", &assembler) == TRUE)
  ASSERT_DATA_EQUALS(ARR({-65}))

  ASSERT(compile_assembly_code(".string \"abc\"xyz\"", &assembler) == TRUE)
  ASSERT_DATA_EQUALS(ARR({'a', 'b', 'c', '"', 'x', 'y', 'z', '\0'}))

  ASSERT(compile_assembly_code(".data   a", &assembler) == FALSE)
  ASSERT(compile_assembly_code(".data   ", &assembler) == FALSE)
  ASSERT(compile_assembly_code(".string \"abc", &assembler) == FALSE)
  ASSERT(compile_assembly_code(".string \"abc\" extra text", &assembler) == FALSE)
  ASSERT(compile_assembly_code("   .mat   [ 0   ] [ 1 ] -65", &assembler) == FALSE)
  ASSERT(compile_assembly_code(".asdf", &assembler) == FALSE)
  ASSERT(compile_assembly_code(".dataa 1, 3", &assembler) == FALSE)
  ASSERT(compile_assembly_code(".stringa \"a\"", &assembler) == FALSE)
  ASSERT(compile_assembly_code(". 1, 3", &assembler) == FALSE)

  return 0;
}
