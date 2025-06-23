#include "./data.h"
#include "./utils.c"
#include <stdio.h>
#include <stdlib.h>

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

  /*If Input file is Unavailable, exit.*/
  if (in == NULL) {
    fprintf(stderr, "Couldn't open Input File\n");
    exit(EXIT_FAILURE);
  }
  /*NOTE: Bug here, We need to also read the line that ends with EOF, since it
   * may be another line.*/
  while (read_line(in, line) != SENTENCE_EOF) {

  }

  fprintf(out, line, "%s");
  return 0;
}
