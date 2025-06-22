#include "../common/linked_list.h"
#include "libs/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Reads a single line from the file that is at most `MAX_LINE` bytes long, and stores it in `line`. */
/* Returns 0 if the line is longer than 80 characters, 1 otherwise. */
int read_line(FILE *file, char line[MAX_LINE]) {
  int count = 0;
  int c;
  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      line[count] = 0;
      return 1;
    }
    if (count >= MAX_LINE) {
      /* The line is longer than 80 characters - read everything until the next newline and return 0. */
      while ((c = getc(file)) != EOF && c != '\n') {
      }
      printf("Error: Line is longer than 80 characters.\n");
      return 0;
    }
    line[count] = (char)c;
    count++;
  }
  line[count] = 0;
  return 1;
}

void print_macro(FILE *out, FILE *in) {
  char line[MAX_LINE] = "";

  while (fgets(line, MAX_LINE, in) && parse_line(line, NULL, 0) != LINE_MCROEND ) {
    fprintf(out, "%s", line);
  }
}

int main(int argc, char *argv[]) {
  FILE *in = stdin;
  FILE *out;
  char line[MAX_LINE];
  char macro_name[MAX_LINE];
  parse_line_status_t status = LINE_NORMAL;
  linked_list_t macro_table = list_init();

  if (argc < 2) {
    fprintf(stderr, "Usage %s input_file [output_file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  in = fopen(argv[1], "r");
  out = fopen("output.asm", "w");

  /* If the input file is unavailable, exit. */
  if (in == NULL) {
    fprintf(stderr, "Couldn't open input file\n");
    exit(EXIT_FAILURE);
  }

  while (!feof(in)) { /* TODO not use feof */
    if (read_line(in, line) == 0) {
      return EXIT_FAILURE;
    }

    status = parse_line(line, macro_name, 1);

    if (status == LINE_NORMAL) {
      /* A line with no special meaning to the preprocessor. We output it as is. */
      fprintf(out, "%s\n", line);
    } else if (status == LINE_MCRO) {
      /* A macro has been defined. We store its offset in the macro table, and skip past all lines until the next `mcroend`. */
      long offset = ftell(in);
      list_add(&macro_table, macro_name, offset);
      print_list(&macro_table);

      do {
        if (!read_line(in, line)) {
          return EXIT_FAILURE;
        }
        status = parse_line(line, macro_name, 0);
      } while (status != LINE_MCROEND);
    } else if (status == LINE_MACROCALL) {
      /* A macro has been called. We check if it exists in the macro table, and if it does, print its contents. */
      long offset = list_get(&macro_table, macro_name);
      if (offset == -1L) {
        printf("No macro named '%s' has been defined.\n", macro_name);
        return EXIT_FAILURE;
      }

      FILE *temp = fopen(argv[1], "r");
      if (fseek(temp, offset, SEEK_SET)) {
        fprintf(stderr, "fseek didn't work while trying to read macro");
        exit(EXIT_FAILURE);
      }

      print_macro(out, temp);
      fclose(temp);
    } else if (status == LINE_ERROR) {
      /* TODO continue to next file */
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
