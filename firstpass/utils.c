#include "./data.h"
#include <string.h>

static const struct KEYWORD_INFO arr[] = {
    {MOV, MOV_VAL, TWO_ARGS}, {CMP, CMP_VAL, TWO_ARGS},
    {ADD, ADD_VAL, TWO_ARGS}, {SUB, SUB_VAL, TWO_ARGS},
    {NOT, NOT_VAL, TWO_ARGS}, {CLR, CLR_VAL, ONE_ARG},
    {LEA, LEA_VAL, ONE_ARG},  {INC, INC_VAL, ONE_ARG},
    {DEC, DEC_VAL, ONE_ARG},  {JMP, JMP_VAL, ONE_ARG},
    {BNE, BNE_VAL, ONE_ARG},  {RED, RED_VAL, ONE_ARG},
    {PRN, PRN_VAL, ONE_ARG},  {JSR, JSR_VAL, ONE_ARG},
    {RTS, RTS_VAL, NO_ARGS},  {STOP, NO_ARGS}};

const int keywords_length = sizeof(arr) / sizeof(struct KEYWORD_INFO);
/*
 * Returns The correspoding binary value of a string keyword.
 * if the keyword given is NOT a known keyword, return -1.
 */
int keyword_to_value(char *token) {

  for (int i = 0; i < keywords_length ; i++) {
    if (!strcmp(token, arr[i].name))
      return arr[i].value;
  }
  return -1;
}
