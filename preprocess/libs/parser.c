#include "parser.h"
#include "../../firstpass/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Reads the next token at the string pointed to by `str`. */
/* If `token` is not NULL, stores a copy of the token's text in it. */
/* Updates `str` so that it will point to the next character after the token that was read. */
/* Returns 1 if a token was read, and 0 otherwise. */
int read_token(char **str, char *token) {
  /* Skip past leading spaces. */
  while (**str && isspace(**str)) {
    (*str)++;
  }

  if (!**str) {
    /* Null terminator was reached. */
    return 0;
  }

  /* Copy characters from `str` to `token` as long as they're not spaces or null. */
  while (**str && !isspace(**str)) {
    if (token) {
      *token = **str;
      token++;
    }
    (*str)++;
  }

  if (token) {
    *token = 0;
  }

  return 1;
}

parse_line_status_t parse_line(char line[MAX_LINE], char *macro_name, int print_errors) {
  char token[MAX_LINE];
  char *cur_line = line;

  if (!read_token(&cur_line, token)) {
    /* An empty line - no action needed. */
    return LINE_NORMAL;
  }

  if (strcmp(token, "mcro") == 0) {
    /* A line that starts with `mcro` begins a macro definition. */
    if (!read_token(&cur_line, macro_name)) {
      if (print_errors) {
        printf("Macro initialization doesn't contain name.\n");
      }
      return LINE_ERROR;
    }

    if (read_token(&cur_line, NULL)) {
      if (print_errors) {
        printf("Extraneous text after macro definition.\n");
      }
      return LINE_ERROR;
    }

    /* Macro names cannot be keywords. */
    if (is_assembly_command(macro_name)) {
      if (print_errors) {
        printf("Macro name cannot be keyword.\n");
      }
      return LINE_ERROR;
    }

    return LINE_MCRO;
  }

  if (strcmp(token, "mcroend") == 0) {
    if (read_token(&cur_line, NULL)) {
      if (print_errors) {
        printf("Extraneous text after `mcroend`.\n");
      }
      return LINE_ERROR;
    }
    return LINE_MCROEND;
  }

  /* If this line is just a single non-keyword token with no tokens after it, it may be a macro call. */
  if (!is_assembly_command(token) && !read_token(&cur_line, NULL)) {
    strcpy(macro_name, token);
    return LINE_MACROCALL;
  }

  return LINE_NORMAL;
}
