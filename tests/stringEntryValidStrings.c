#include "common.h"

int main() {
  /*we need assembler here, we're not using it because we're checking for errors.*/
  assembler_t assembler = assembler_create();


  char empty[MAX_LINE] = ".string";

  char no_begin[MAX_LINE] = ".string \"a";
  char no_end[MAX_LINE] = ".string b\"";

  char letter_after_ending_of_string[MAX_LINE] = ".string \"\"a";
  char letter_before_ending_of_string[MAX_LINE] = ".string b\"\"";


  bool_t no_begin_flag = compile_assembly_code(no_begin, &assembler);
  ASSERTM(no_begin_flag == FALSE, "String doesn't contain two \". ");

  bool_t no_end_flag = compile_assembly_code(no_end, &assembler);
  ASSERTM(no_end_flag == FALSE, "String doesn't contain two \". ");

  bool_t letter_after_ending_of_string_flag = compile_assembly_code(letter_after_ending_of_string, &assembler);
  ASSERTM(letter_after_ending_of_string_flag == FALSE, "String can't contain ascii characters AFTER string ends\". ");


  bool_t letter_before_ending_of_string_flag = compile_assembly_code(letter_before_ending_of_string, &assembler);
  ASSERTM(letter_before_ending_of_string_flag == FALSE, "String can't contain ascii characters AFTER string ends\". ");


  bool_t empty_flag = compile_assembly_code(empty, &assembler);
  ASSERTM(empty_flag == FALSE, "Empty string needs to throw an error.");


  /*TODO: Is there a reason to check that ALL characters inside the string are valid ASCII characters?*/
}
