#include <stdio.h>
#include <string.h>

#define ASSERT(condition, message, is_critical)                                \
  if (!(condition)) {                                                          \
    printf("%s", message);                                                     \
    if (is_critical)                                                           \
      exit(1);                                                                 \
    return 1;                                                                  \
  }

enum LINE_TYPE { INFORMATIVE, COMMAND, WHITESPACE, COMMENT, UNKNOWN };

enum LINE_TYPE line_type(char *line) {
  int len = strlen(line);
  if (*line == ';') {
    return COMMENT;
  }

  /*TODO: This function checks what types of command we're in, label has to be
     checked else where (since it's optional.) */
  // if (*(line + (len - 1)) == ':') {
  //   /* Checks if the last value is a colon, meaning it's a label.*/
  //   return
  // }

  else if (*line == '.') {
    /* This is  For  Informative sentence, for whenever we receieve */
    return INFORMATIVE;
  }

  /*TODO: Create a a checker for Command sentences. */
  return UNKNOWN;
}

int is_label(char *token) {
  int len = strlen(token);

  return *(token + (len - 1)) == ':';
}

/*FIXME: add global header file for MAX_LINE length, so i won't have to set it
 * manually here.*/
void parse_line(char line[81]) {
  /* Does Line Contain Label? (Something like variable name)*/
  char *token = strtok(line, " \t");
  char *label = NULL;
  /*If The line contains semi-colon, we shall ignore it since it's a a
   * comment.*/
  if (*token == ';')
    return;

  if (is_label(token)) {
    memcpy(label, token, strlen(token) - 1);
    printf("Label is: %s", label);
  }
}
