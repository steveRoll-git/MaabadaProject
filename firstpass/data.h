enum KEYWORD {
  MOV = 0,
  CMP = 1,
  ADD = 2,
  SUB = 3,
  NOT = 4,
  CLR = 5,
  LEA = 6,
  INC = 7,
  DEC = 8,
  JMP = 9,
  BNE = 10,
  RED = 11,
  PRN = 12,
  JSR = 13,
  RTS = 14,
  STOP = 15
};

enum ARGS { NO_ARGS = 0, ONE_ARG = 1, TWO_ARGS = 2 };

#define KEYWORD_ARG                                                            \
  {TWO_ARGS, TWO_ARGS, TWO_ARGS, TWO_ARGS, TWO_ARGS, ONE_ARG,                  \
   ONE_ARG,  ONE_ARG,  ONE_ARG,  ONE_ARG,  ONE_ARG,  ONE_ARG,                  \
   ONE_ARGS, ONE_ARGS, NO_ARGS,  NO_ARGS};

