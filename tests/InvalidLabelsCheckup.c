#include "common.h"

char str1[MAX_LINE] = "a_mc:";
char str2[MAX_LINE] = "stop:";
char str3[MAX_LINE] = "mov:";
char str4[MAX_LINE] = "0R2:";
char str5[MAX_LINE] = "@mov:";
char str6[MAX_LINE] = "r2:";
char str7[MAX_LINE] = "r3:";
char str8[MAX_LINE] = "AF!!:";

char str9[MAX_LINE] = ":";

void test(char *s, char *m1, char *m2) {
  assembler_t assembler = assembler_create();

  if (m1 != NULL) {
    ASSERTM(is_label(s) == TRUE, m1);
  }

  if (m1 != NULL && m2 != NULL) {
    ASSERTM(is_label(s) == TRUE, m1);
    ASSERTM(is_label_valid(s, &assembler) == FALSE, m2);
  }
}

int main(void) {
  test(str1, "Label Contains :", "labels cannot contain special characters.");

  test(str2, "Label Contains :", "instructions (stop) cant be label");


  test(str3, "Label Contains :", "instructions (mov) cant be a label");


  ASSERTM(is_label(str4) == FALSE, "First character must be alphabetical, cannot be a number");

  ASSERTM(is_label(str6) == FALSE, "First character must be alphabetical, cannot be a character.");

  test(str6, "Label Contains :", "label cannot be a register.");

  test(str7, "Label Contains :", "label cannot be a register.");

  test(str8, "Label Contains :", "label isnt valid because it cannot contain special characters.");

  ASSERTM(is_label(str9) == FALSE, "Label name cant be empty lol");
}
