#include <stdio.h>
#include <stdlib.h>
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

  /*TODO: This function checks what types of command we're in, label has to be
     checked else where (since it's optional.) */
  // if (*(line + (len - 1)) == ':') {
  //   /* Checks if the last value is a colon, meaning it's a label.*/
  //   return
  // }

  if (*line == '.') {
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

/*TODO: There's a bug here that token equals to the label name even though we change its value everytime with strtok.*/
void parse_line(char line[81]) {
  /* Does Line Contain Label? (Something like variable name)*/
  printf("This is the Line we're working on: %s\n", line);
  char *token = strtok(line, " \t");
  char *label = NULL;
  enum LINE_TYPE l_type;

  /*If The line contains semi-colon, we shall ignore it since it's a a
   * comment.*/
  if (*token == ';')
    return;

  /* if after using strtok , the only letter we get out of it is the newline
   * letter, it means the entire line was whitespaces, and we shall ignore it.*/
  if (*token == '\n')
    return;

  printf("Executing Parser :) \n");
  printf("Token Currently: %s\n", token);

  if (is_label(token)) {
    label = (char *)malloc(strlen(token));
    strncpy(label, token, strlen(token) - 1); ;
    printf("Label currently is : %s\n", label);

    token = strtok(NULL, " \t");
  }

  l_type = line_type(token);

  while (token != NULL) {
    printf("TOKEN: %s\n", token);
    token = strtok(NULL, " \t");
  }

  free(label);
}
