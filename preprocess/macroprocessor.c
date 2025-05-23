#include "libs/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./parser.h"
int main(int argc, char *argv[]) {
  FILE *in = stdin, *out;
  char line[MAX_LINE];
  char *token;

  if (argc < 2) {
    fprintf(stderr, "Usage %s input_file [output_file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Open Read File, check that it works wonderfully.*/
  in = fopen(argv[1], "r");
  out = fopen("output.asm", "w");

  /*If Input file is Unavailable, exist.*/
  if (in == NULL) {
    fprintf(stderr, "Couldn't open Input File\n");
    exit(EXIT_FAILURE);
  }

  while (fgets(line, MAX_LINE, in) != NULL) {

    /* If line starts with this character, it means it's  a comment, ignore it
     * and do not parse. */
    // printf("line: %s\n", line);
    parse_line(line);
  }

  return EXIT_SUCCESS;
}
