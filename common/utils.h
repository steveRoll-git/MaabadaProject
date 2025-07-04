#ifndef FIRSTPASS_UTILS_H
#define FIRSTPASS_UTILS_H

#include <stdio.h>
#include "../common/data.h"
#include "../datatypes/assembler.h"

extern instruction_t instructions[];

typedef enum token_kind_t {
  /* Not a valid token. */
  TOKEN_INVALID = 0,
  /* A number token, e.g. 123, -456, +789 */
  TOKEN_NUMBER,
  /* A comma ','. */
  TOKEN_COMMA,
  /* A hash '#'. */
  TOKEN_HASH,
  /* A colon ':'. */
  TOKEN_COLON,
  /* An identifier - any name that isn't a reserved word. */
  TOKEN_IDENT,
  /* A name of a register: 'r0' through 'r7'. */
  TOKEN_REGISTER,
  /* The 'mcro' preprocessor keyword. */
  TOKEN_MCRO,
  /* The 'mcroend' preprocessor keyword. */
  TOKEN_MCROEND,
  /* The name of an instruction. */
  TOKEN_INSTRUCTION,
  /* The end of the current line. */
  TOKEN_END
} token_kind_t;

typedef struct token_t {
  /* What kind of token this is. */
  token_kind_t kind;
  /* This token's contents. */
  char value[MAX_LINE];
  /* If `kind` is `TOKEN_REGISTER`, this stores the register's index, from 0 to 7. */
  char register_index;
  /* If `kind` is `TOKEN_INSTRUCTION`, this stores a pointer to the instruction. */
  instruction_t *instruction;
} token_t;

/* Reads the next token at the string pointed to by `s`, after skipping leading spaces. */
/* Updates `s` so that it will point to the next character after the token that was read. */
token_t read_token(char **s);

/* Given an instruction's name, returns information about that instruction. */
/* If no instruction by that name exists, returns an instruction with -1 as its opcode. */
instruction_t *get_instruction(char *token);

/**
 * Given a token inside an assembly command, check if it's a label.
 * returns -1 if the string given is NULL.
 * otherwise :
 * returns 1 if its a label, 0 otherwise.
 *
 * NOTE: remember this function only checks if its a label, to check if the
 * label string is valid, please clal is_label_valid.
 */
int is_label(char *token);

/**
 * Checks if label is valid, by checking the following things:
 * 1) is the first character alphanumeric? [a-zA-Z].
 * 2) is the label name already used as a macro name?
 * 3) is the label name already used as a label?
 *
 * if any of those checks are true, the label isn't valid and returns 0.
 * if it's valid it returns 1.
 */
int is_label_valid(char *label, assembler_t *assembler);

/*
 * specific parser we've created to read lines,
 * the command reads lines from a given file and stops  the moment we the end of
 * a line (Meaning '\n', or EOF). if the line received is longer than 80 bytes,
 * the buffer won't receive any more bytes and return 0. otherwise it will
 * return 1.
 * */

sentence_t read_line(FILE *file, char line[MAX_LINE]);

/* Returns whether the string is the name of an assembly instruction. */
int is_assembly_instruction(char *token);

/* Returns the length of the identifier pointed to by the given string. */
/* An identifier is a word that starts with a letter, and then only contains letters or digits. */
int identifier_length(const char *ident);

int is_register(const char *token);

int keyword_to_arg_amount(char *token);
#endif
