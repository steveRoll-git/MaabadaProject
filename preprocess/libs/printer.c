
#include "./parser.h"
#include <string.h>

int should_print(char line[MAX_LINE]) {
  char temp[MAX_LINE];
  strcpy(temp, line);

  strtok(temp, " \t");

  return !((*temp == ';') || (*temp == '\n'));
}

void print_line(FILE *out, char *line) { fprintf(out, "%s", line); }

/*Prints Macro to our Output File*/
void print_macro(FILE *out, FILE *in) {
  char line[MAX_LINE] = "";

  fgets(line, MAX_LINE, in);
  while (!strstr(line, "mcroend")) {
    if (should_print(line))
      fprintf(out, "%s", line);
  }
}
