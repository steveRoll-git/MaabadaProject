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

static word_t word;

int main(void) {

  read_word(&str1, &word);
  ASSERTM(word.kind == WORD_IDENTIFIER, "labels cannot contain special characters.");

  read_word(&str2, &word);
  ASSERTM(word.kind == WORD_INSTRUCTION, "Should return its an instruction.")

  read_word(&str3, &word);
  ASSERTM(word.kind == WORD_INSTRUCTION, "Should return its an instruction.")

  read_word(&str4, &word);
  ASSERTM(word.kind == WORD_NONE, "");

  read_word(&str5, &word);
  ASSERTM(word.kind == WORD_NONE, "");

  read_word(&str6, &word);
  ASSERTM(word.kind == WORD_REGISTER, "");

  read_word(&str7, &word);
  ASSERTM(word.kind == WORD_REGISTER, "");

  read_word(&str8, &word);
  ASSERTM(word.kind == WORD_IDENTIFIER, "");

  read_word(&str9, &word);
  ASSERTM(word.kind == WORD_NONE, "");

  return 0;
}
