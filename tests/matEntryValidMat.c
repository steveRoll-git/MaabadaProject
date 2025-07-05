#include "common.h"

char empty[MAX_LINE] = ".mat";
char q1[MAX_LINE] = ".mat [";
char q2[MAX_LINE] = ".mat [#5";
char q3[MAX_LINE] = ".mat []";
char q4[MAX_LINE] = ".mat [#5";
char q5[MAX_LINE] = ".mat [2]";
char q6[MAX_LINE] = ".mat [5";
char q7[MAX_LINE] = ".mat [5][";
char q8[MAX_LINE] = ".mat [9][]";
char q9[MAX_LINE] = ".mat [10][r1]";
char q10[MAX_LINE] = ".mat [10][-5]";
char q11[MAX_LINE] = ".mat [-1][-9]";
char q12[MAX_LINE] = ".mat [0][0]";
char q13[MAX_LINE] = ".mat [1][a]";
char q14[MAX_LINE] = ".mat [a][b]";
char q15[MAX_LINE] = ".mat [-5][0]";
char q16[MAX_LINE] = ".mat [1][2] 1, 2, 10";
char q17[MAX_LINE] = ".mat [2][2] 1, 2, 3, -999";
char q18[MAX_LINE] = ".mat [5][2] 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11";
char q19[MAX_LINE] = ".mat [2][2] ,";
char q20[MAX_LINE] = ".mat [1][2] ,,";

void test(char str[MAX_LINE], char *message) {
  assembler_t assembler = assembler_create();
  bool_t flag = compile_assembly_code(str, &assembler);

  ASSERTM(flag == FALSE, message);
}

int main(void) {
  test(q1, "missing row initialization");
  test(q2, "invalid row value");
  test(q3, "invalid row value");
  test(q4, "invalid row value");
  test(q5, "missing row initialization");
  test(q6, "unfinished row initialiation");

  test(q7, "invalid column initialization");
  test(q8, "invalid column value");
  test(q9, "invalid column value");

  test(q10, "matrice columns cannot be negative size");
  test(q11, "matrice row cannot be negative size");

  test(q12, "matrice row cannot be negative size");
  test(q13, "illegal column value");
  test(q14, "illegal row value");
  test(q15, "matrice row cannot be negative size");

  test(q16, "should fail because there are more values than storage (array overflow)");
  test(q17, "should fail because integer value -999 is too big for int buffer, should throw err.");

  test(q18, "should fail because there are more values than storage (array overflow)");
  test(q19, "illegal comma");
  test(q20, "illegal comma");


  return 0;
}
