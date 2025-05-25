#include "./libs/datatypes.h"
#include "./libs/printer.h"
#include "libs/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *in = stdin, *out;
  char line[MAX_LINE];
  char *token;
  llm_t *macro_table = NULL;
  int status = NORMAL;
  char *lineR = (char *)malloc(MAX_LINE);

  macro_table = (llm_t *)malloc(sizeof(llm_t));
  macro_table->macro.name = "";

  if (argc < 2) {
    fprintf(stderr, "Usage %s input_file [output_file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Open Read File, check that it works wonderfully.*/
  in = fopen(argv[1], "r");
  out = fopen("output.asm", "w");

  /*If Input file is Unavailable, exit.*/
  if (in == NULL) {
    fprintf(stderr, "Couldn't open Input File\n");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, MAX_LINE, in) != NULL) {
    /* If line starts with this character, it means it's  a comment, ignore it
     * and do not parse. */
    printf("Line: %s\n", line);
    strcpy(lineR, line);

    status = parse_line(line, macro_table, in, out);

    if (status == IN_MACRO) {
      do {
        if (fgets(line, MAX_LINE, in) == NULL)
          break;
        strcpy(lineR, line);
        printf("line: %s\n", line);
        status = parse_line(line, macro_table, in, out);
      } while (status != MCROEND);

      print_llm(macro_table);
    }
    if (status == NORMAL) {
      fprintf(out, "%s", lineR);
    }
    if (status == MCALL){
      printf("Inside MCall\n");
      token = strtok(lineR, " \t\n");

      long offset = llm_contains(macro_table, token);
      FILE* temp = fopen(argv[1], "r");
      if(fseek(temp, offset, SEEK_SET)){
        fprintf(stderr, "fseek didn't work while trying to read file value");
        exit(EXIT_FAILURE);
      }
      print_macro(out, temp);
      fclose(temp);

    }
  }
  return EXIT_SUCCESS;
}
