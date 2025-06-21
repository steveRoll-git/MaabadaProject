#define MOV "mov"
#define CMP "cmp"
#define ADD "add"
#define SUB "sub"
#define NOT "not"
#define CLR "clr"
#define LEA "lea"
#define INC "inc"
#define DEC "dec"
#define JMP "jmp"
#define BNE "bne"
#define RED "red"
#define PRN "prn"
#define JSR "jsr"
#define RTS "rts"
#define STOP "stop"

enum KEYWORD {
  MOV_VAL = 0,
  CMP_VAL = 1,
  ADD_VAL = 2,
  SUB_VAL = 3,
  NOT_VAL = 4,
  CLR_VAL = 5,
  LEA_VAL = 6,
  INC_VAL = 7,
  DEC_VAL = 8,
  JMP_VAL = 9,
  BNE_VAL = 10,
  RED_VAL = 11,
  PRN_VAL = 12,
  JSR_VAL = 13,
  RTS_VAL = 14,
  STOP_VAL = 15
};

enum ARGS { NO_ARGS = 0, ONE_ARG = 1, TWO_ARGS = 2 };

struct KEYWORD_INFO {
  char *name;
  KEYWORD value;
  ARGS arg_amount;
};
