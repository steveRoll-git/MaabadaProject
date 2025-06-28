#ifndef DATA_H
#define DATA_H

/* The maximum size of a single line in bytes (80 characters and a null terminator.) */
#define MAX_LINE 81

#define INSTRUCTION_MOV "mov"
#define INSTRUCTION_CMP "cmp"
#define INSTRUCTION_ADD "add"
#define INSTRUCTION_SUB "sub"
#define INSTRUCTION_NOT "not"
#define INSTRUCTION_CLR "clr"
#define INSTRUCTION_LEA "lea"
#define INSTRUCTION_INC "inc"
#define INSTRUCTION_DEC "dec"
#define INSTRUCTION_JMP "jmp"
#define INSTRUCTION_BNE "bne"
#define INSTRUCTION_RED "red"
#define INSTRUCTION_PRN "prn"
#define INSTRUCTION_JSR "jsr"
#define INSTRUCTION_RTS "rts"
#define INSTRUCTION_STOP "stop"

#define DATATYPE_DATA ".data"
#define DATATYPE_STRING ".string"
#define DATATYPE_MAT ".mat"
#define DATATYPE_ENTRY ".entry"
#define DATATYPE_EXTERN ".extern"

typedef enum datatype_t { DATA, STRING, MAT, ENTRY, EXTERN, UNKNOWN } datatype_t;

typedef enum opcode_t {
  OPCODE_MOV = 0,
  OPCODE_CMP = 1,
  OPCODE_ADD = 2,
  OPCODE_SUB = 3,
  OPCODE_NOT = 4,
  OPCODE_CLR = 5,
  OPCODE_LEA = 6,
  OPCODE_INC = 7,
  OPCODE_DEC = 8,
  OPCODE_JMP = 9,
  OPCODE_BNE = 10,
  OPCODE_RED = 11,
  OPCODE_PRN = 12,
  OPCODE_JSR = 13,
  OPCODE_RTS = 14,
  OPCODE_STOP = 15
} opcode_t;

typedef enum args_t { NO_ARGS = 0, ONE_ARG = 1, TWO_ARGS = 2 } args_t;

typedef struct instruction_t {
  char *name;
  opcode_t value;
  args_t arg_amount;
} instruction_t;

typedef enum sentence_t {
  SENTENCE_ERR_BUFF_OVERFLOW = 0,
  SENTENCE_NEW_LINE = 1,
  SENTENCE_EOF = 2,
} sentence_t;

#endif
