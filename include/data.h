#ifndef DATA_H
#define DATA_H

/* The maximum size of a single line in bytes (80 characters and a null terminator.) */
#define MAX_LINE 81

/* The maximum size of a label in bytes. */
#define MAX_LABEL 31

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

#define DIRECTIVE_DATA "data"
#define DIRECTIVE_STRING "string"
#define DIRECTIVE_MAT "mat"
#define DIRECTIVE_ENTRY "entry"
#define DIRECTIVE_EXTERN "extern"
/* The longest directive is 6 characters long. */
#define DIRECTIVE_MAX_LEN 6

/* The smallest value an immediate operand can have. */
#define IMMEDIATE_MIN -128
/* The largest value an immediate operand can have. */
#define IMMEDIATE_MAX 127

/* The largest possible address in an assembly program. */
#define ADDRESS_MAX 255

/* A boolean - represents true or false. */
typedef enum { FALSE = 0, TRUE = 1 } bool_t;

/* The type needed to represent a single machine word. */
/* A word in the virtual machine is 10 bits; The closest type we have to this is `short int` which is 16 bits. */
typedef short int machine_word_t;

/* The different kinds of directives. */
typedef enum directive_kind_t {
  /* `.data` directive. */
  DIRECTIVE_KIND_DATA,
  /* `.string` directive. */
  DIRECTIVE_KIND_STRING,
  /* `.mat` directive. */
  DIRECTIVE_KIND_MAT,
  /* `.entry` directive. */
  DIRECTIVE_KIND_ENTRY,
  /* `.extern` directive. */
  DIRECTIVE_KIND_EXTERN,
  /* Invalid directive. */
  DIRECTIVE_KIND_UNKNOWN
} directive_kind_t;

/* The opcodes of each instruction. */
typedef enum opcode_t {
  OPCODE_MOV,
  OPCODE_CMP,
  OPCODE_ADD,
  OPCODE_SUB,
  OPCODE_LEA,
  OPCODE_CLR,
  OPCODE_NOT,
  OPCODE_INC,
  OPCODE_DEC,
  OPCODE_JMP,
  OPCODE_BNE,
  OPCODE_JSR,
  OPCODE_RED,
  OPCODE_PRN,
  OPCODE_RTS,
  OPCODE_STOP
} opcode_t;

/* The different kinds of encoding that can appear in a word's "ARE" bits. */
typedef enum encoding_kind_t {
  ENCODING_ABSOLUTE,
  ENCODING_EXTERNAL,
  ENCODING_RELOCATABLE,
} encoding_kind_t;

/* The number of arguments that an instruction can have. */
typedef enum num_args_t { NO_ARGS = 0, ONE_ARG = 1, TWO_ARGS = 2 } num_args_t;

/* Stores information about an instruction. */
typedef struct instruction_info_t {
  /* The instruction's name. */
  char *name;
  /* The instruction's opcode. */
  opcode_t opcode;
  /* How many operands this instruction needs. */
  num_args_t arg_amount;
  /* Whether the destination operand supports the immediate addressing mode. */
  bool_t dst_immediate;
  /* Whether the source operand supports the immediate or register addressing modes. */
  bool_t src_immediate_register;
} instruction_info_t;

/* Given an instruction's name, returns information about that instruction. */
/* If no instruction by that name exists, returns NULL. */
instruction_info_t *get_instruction(char *token);

#endif
