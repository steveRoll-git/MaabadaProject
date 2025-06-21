#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 80

int main(int argc, char *argv[]) {
  FILE *in = stdin, *out;
  char line[MAX_LINE];
  char dup[MAX_LINE];
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

  while (fgets(line, MAX_LINE, in)) {

    if (!strchr(line, '\n')) {
      fprintf(stderr, "Line is larger than 80 characters");
      exit(1);
    }

    if (!sscanf(line, "%s", dup)) {
      /* Whitespace Line, Ignore.*/
      continue;
    }
    printf("\"%s\" Length of string %lu\n", dup, strlen(dup));
    /* Comment line, Can ignore.*/
    if (dup[0] == ';') {
      continue;
    }
    fprintf(out, line, "%s");
  }
  return 0;
}
