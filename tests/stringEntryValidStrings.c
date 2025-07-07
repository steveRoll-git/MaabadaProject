#include "common.h"

static statement_t statement;

int main() {
  char empty[MAX_LINE] = ".string";

  char no_begin[MAX_LINE] = ".string \"a";
  char no_end[MAX_LINE] = ".string b\"";

  char letter_after_ending_of_string[MAX_LINE] = ".string \"\"a";
  char letter_before_ending_of_string[MAX_LINE] = ".string b\"\"";

  char one_c[MAX_LINE] = ".string \"";

  result_t no_begin_flag = parse_statement(no_begin, &statement);
  ASSERTM(no_begin_flag != SUCCESS, "String doesn't contain two \". ");

  result_t no_end_flag = parse_statement(no_end, &statement);
  ASSERTM(no_end_flag != SUCCESS, "String doesn't contain two \". ");

  result_t letter_after_ending_of_string_flag = parse_statement(letter_after_ending_of_string, &statement);
  ASSERTM(letter_after_ending_of_string_flag != SUCCESS, "String can't contain ascii characters AFTER string ends\". ");


  result_t letter_before_ending_of_string_flag = parse_statement(letter_before_ending_of_string, &statement);
  ASSERTM(letter_before_ending_of_string_flag != SUCCESS,
          "String can't contain ascii characters AFTER string ends\". ");

  result_t empty_flag = parse_statement(empty, &statement);
  ASSERTM(empty_flag != SUCCESS, "Empty string needs to throw an error.");

  result_t c_flag = parse_statement(one_c, &statement);
  ASSERTM(c_flag != SUCCESS, "Empty string needs to throw an error.");

  return 0;
  /*TODO: Is there a reason to check that ALL characters inside the string are valid ASCII characters?*/
}
