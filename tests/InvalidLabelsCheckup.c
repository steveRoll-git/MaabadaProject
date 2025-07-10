#include "common.h"

char *str1 = "a_mc:";
char *str2 = "stop:";
char *str3 = "mov:";
char *str4 = "0R2:";
char *str5 = "@mov:";
char *str6 = "r2:";
char *str7 = "r3:";
char *str8 = "AF!!:";
char *str9 = ":";

int main(void) {

  ASSERTM(read_word(&str1).kind == WORD_IDENTIFIER, "labels cannot contain special characters.");

  ASSERTM(read_word(&str2).kind == WORD_INSTRUCTION, "Should return its an instruction.")

  ASSERTM(read_word(&str3).kind == WORD_INSTRUCTION, "Should return its an instruction.")

  ASSERTM(read_word(&str4).kind == WORD_NONE, "");

  ASSERTM(read_word(&str5).kind == WORD_NONE, "");

  ASSERTM(read_word(&str6).kind == WORD_REGISTER, "");

  ASSERTM(read_word(&str7).kind == WORD_REGISTER, "");

  ASSERTM(read_word(&str8).kind == WORD_IDENTIFIER, "");

  ASSERTM(read_word(&str9).kind == WORD_NONE, "");

  return 0;
}
