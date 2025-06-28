#include "../datatypes/linked_list.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  linked_list_t macro_table = list_init();

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
    } else if (status == LINE_MCRO) {
      /* A macro has been defined. We store its offset in the macro table, and skip past all lines until the next `mcroend`. */
      long offset = ftell(in);
      list_add(&macro_table, macro_name, offset);
      print_list(&macro_table);

      do {
        if (!read_line(in, line)) {
          return 0;
        }
        status = parse_line(line, macro_name, 0);
      } while (status != LINE_MCROEND);
    } else if (status == LINE_MACROCALL) {
      /* A macro has been called. We check if it exists in the macro table, and if it does, print its contents. */
      long offset = list_get(&macro_table, macro_name);
      if (offset == -1L) {
        printf("No macro named '%s' has been defined.\n", macro_name);
        return 0;
      }

      FILE *temp = fopen(input_file_path, "r");
      if (fseek(temp, offset, SEEK_SET)) {
        fprintf(stderr, "fseek didn't work while trying to read macro");
        return 0;
      }

      print_macro(out, temp);
      fclose(temp);
    } else if (status == LINE_ERROR) {
      /* TODO continue to next file */
      return 0;
    }
  }
  return 1;
}
