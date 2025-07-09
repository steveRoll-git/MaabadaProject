#ifndef FIRSTPASS_UTILS_H
#define FIRSTPASS_UTILS_H

#include <stdio.h>
#include "data.h"
#include "utils.h"

extern instruction_info_t instructions[];

typedef enum word_kind_t {
  /* The text that was attempted to be read is not a word. */
  WORD_NONE,
  /* The 'mcro' preprocessor keyword. */
  WORD_MCRO,
  /* The name of an instruction. */
  WORD_MCROEND,
  /* The name of an instruction. */
  WORD_INSTRUCTION,
  /* A name of a register: 'r0' through 'r7'. */
  WORD_REGISTER,
  /* Any name that isn't a reserved word. */
  WORD_IDENTIFIER
} word_kind_t;

typedef struct word_t {
  /* What kind of word this is. */
  word_kind_t kind;
  /* This word's contents. */
  char value[MAX_LINE];
  /* If `kind` is `WORD_REGISTER`, this stores the register's index, from 0 to 7. */
  char register_index;
  /* If `kind` is `WORD_INSTRUCTION`, this stores a pointer to the instruction. */
  instruction_info_t *instruction_info;
} word_t;

/* Reads the next word (a sequence of alphanumeric characters that starts with a letter) at the string pointed to by
 * `s` */
/* Updates `s` so that it will point to the next character after the word that was read. */
word_t read_word(char **s);

/* Copies a character from the string in `src` to the string in `dest`, and advances both of them by one character. */
void add_char(char **dest, char **src);

/* Given an instruction's name, returns information about that instruction. */
/* If no instruction by that name exists, returns NULL. */
instruction_info_t *get_instruction(char *token);

/*
 * specific parser we've created to read lines,
 * the command reads lines from a given file and stops  the moment we the end of
 * a line (Meaning '\n', or EOF). if the line received is longer than 80 bytes,
 * the buffer won't receive any more bytes and return 0. otherwise it will
 * return 1.
 * */

sentence_t read_line(FILE *file, char line[MAX_LINE]);

/* Moves `*s` to point at the next non-space character. */
void skip_spaces(char **s);

/* Returns whether there are no more non-space characters in `s`. */
int is_end(char *s);

#endif
