#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/table.h"

/* The various kinds of lines that the preprocessor's `parse_line` can give. */
typedef enum {
  LINE_NORMAL, /* A line with no special meaning to the preprocessor. */
  LINE_MCRO, /* A line that starts a macro definition with `mcro`. */
  LINE_MCROEND, /* A line that ends a macro definition with `mcroend`. */
  LINE_MACROCALL, /* A line that calls a macro by its name. */
  LINE_ERROR /* Indicates that an error occurred while reading this line. */
} parse_line_status_t;

parse_line_status_t parse_line(char line[MAX_LINE], char *macro_name, int print_errors) {
  word_t word;
  char *cur_line = line;

  word = read_word(&cur_line);

  if (word.kind == WORD_MCRO) {
    /* A line that starts with `mcro` begins a macro definition. */
    word = read_word(&cur_line);
    if (word.kind != WORD_IDENTIFIER) {
      if (print_errors) {
        printf("Macro initialization doesn't contain a valid name.\n");
      }
      return LINE_ERROR;
    }

    if (macro_name) {
      strcpy(macro_name, word.value);
    }

    if (!is_end(cur_line)) {
      if (print_errors) {
        printf("Extraneous text after macro definition.\n");
      }
      return LINE_ERROR;
    }

    return LINE_MCRO;
  }

  if (word.kind == WORD_MCROEND) {
    if (!is_end(cur_line)) {
      if (print_errors) {
        printf("Extraneous text after `mcroend`.\n");
      }
      return LINE_ERROR;
    }
    return LINE_MCROEND;
  }

  /* If this line is just a single non-keyword token with no tokens after it, it may be a macro call. */
  if (word.kind == WORD_IDENTIFIER && is_end(cur_line)) {
    if (macro_name) {
      strcpy(macro_name, word.value);
    }
    return LINE_MACROCALL;
  }

  return LINE_NORMAL;
}
void print_macro(FILE *out, FILE *in) {
  char line[MAX_LINE] = "";

  while (fgets(line, MAX_LINE, in) && parse_line(line, NULL, 0) != LINE_MCROEND) {
    fprintf(out, "%s", line);
  }
}

int preprocess(char *input_file_path, char *output_file_path) {
  FILE *in;
  FILE *out;
  char line[MAX_LINE];
  char macro_name[MAX_LINE];
  parse_line_status_t status = LINE_NORMAL;
  table_t *macro_table = table_create(sizeof(long));

  in = fopen(input_file_path, "rb");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    printf("Couldn't open input file\n");
    return 0;
  }

  out = fopen(output_file_path, "w");
  /* TODO check if out is null */

  while (!feof(in)) {
    if (read_line(in, line) == 0) {
      return 0;
    }

    status = parse_line(line, macro_name, 1);

    if (status == LINE_NORMAL) {
      /* A line with no special meaning to the preprocessor. We output it as is. */
      fprintf(out, "%s\n", line);
    }
    else if (status == LINE_MCRO) {
      /* A macro has been defined. We store its offset in the macro table, and skip past all lines until the next
       * `mcroend`. */
      long offset = ftell(in);
      TABLE_ADD(macro_table, macro_name, offset);

      do {
        if (read_line(in, line) != SENTENCE_NEW_LINE) {
          return 0;
        }
        status = parse_line(line, macro_name, 0);
      }
      while (status != LINE_MCROEND);
    }
    else if (status == LINE_MACROCALL) {
      /* A macro has been called. We check if it exists in the macro table, and if it does, print its contents. */
      long *offset = table_get(macro_table, macro_name);
      if (offset == NULL) {
        printf("No macro named '%s' has been defined.\n", macro_name);
        return 0;
      }

      FILE *temp = fopen(input_file_path, "r");
      if (fseek(temp, *offset, SEEK_SET)) {
        fprintf(stderr, "fseek didn't work while trying to read macro");
        return 0;
      }

      print_macro(out, temp);
      fclose(temp);
    }
    else if (status == LINE_ERROR) {
      /* TODO continue to next file */
      return 0;
    }
  }
  return 1;
}
