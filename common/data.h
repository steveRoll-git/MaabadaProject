#ifndef DATA_H
#define DATA_H

/* The maximum size of a single line in bytes (80 characters and a null terminator.) */
#define MAX_LINE 81

/* The maximum length of a label. */
#define MAX_LABEL 30

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

#define DIRECTIVE_DATA ".data"
#define DIRECTIVE_STRING ".string"
#define DIRECTIVE_MAT ".mat"
#define DIRECTIVE_ENTRY ".entry"
#define DIRECTIVE_EXTERN ".extern"

typedef enum directive_kind_t {
  DIRECTIVE_KIND_DATA,
  DIRECTIVE_KIND_STRING,
  DIRECTIVE_KIND_MAT,
  DIRECTIVE_KIND_ENTRY,
  DIRECTIVE_KIND_EXTERN,
  DIRECTIVE_KIND_UNKNOWN
} directive_kind_t;

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
  opcode_t opcode;
  args_t arg_amount;
} instruction_t;

typedef enum sentence_t {
  SENTENCE_ERR_BUFF_OVERFLOW = 0,
  SENTENCE_NEW_LINE = 1,
  SENTENCE_EOF = 2,
} sentence_t;

typedef enum { FALSE = 0, TRUE = 1 } bool_t;

#endif
