#include "parser.h"
#include "./datatypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum LINE_TYPE { INFORMATIVE, COMMAND, WHITESPACE, COMMENT, UNKNOWN };

enum LINE_TYPE get_line_type(char *line) {
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

/*Shorted for Is Token Keyword?
 * Checks if a token's value is a known keyword that we've set inside the
 * KEYWORDS values inside the parser.
 */
int itk(char *token) {
  int i = 0;
  char *keywords[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
                      "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

  int size = sizeof(keywords) / sizeof(keywords[0]);

  for (i = 0; i < size; i++)
    printf("%s", keywords[i]);
  if (!strcmp(token, keywords[i]))
    return 1;

  return 0;
}

int is_label(char *token) {
  int len = strlen(token);
  return *(token + (len - 1)) == ':';
}

void parse_line(char line[MAX_LINE]) {
  /* Does Line Contain Label? (Something like variable name  printf("This is the
   * Line we're working on: %s\n", line); */
  char *token = strtok(line, " \t");
  enum LINE_TYPE l_type;

  /*If The line contains semi-colon, we shall ignore it since it's a a
   * comment.*/
  if (*token == ';')
    return;

  /* if after using strtok , the only letter we get out of it is the newline
   * letter, it means the entire line was whitespaces, and we shall ignore it.*/
  if (*token == '\n')
    return;

  /* Check If String is A macro Initialization call */
  if (strcmp(token, "mcro") == 0) {
    printf("We found a macro!\n");
    /*We Receieve second token (mcro name)*/
    token = strtok(NULL, " \t");

    /* Error! Our Entire line only contained the keyword mcro, we haven't gotten
     * a name!*/
    if (*token == '\n') {
      fprintf(stderr, "Mcro Initialization doesn't contain name.");
      exit(EXIT_FAILURE);
    }

    /* Mcro's name is a known keyword, needs to return an error. */

    if (itk(token)) {
      fprintf(stderr, "Macro Name cannot be Keyword");
      exit(EXIT_FAILURE);
    }

    /*TODO: After Checking the token value and seeing it's legit, we need to
     * 1) Check if the value is already initialized, meaning its name was
          already used somewhere.
     * 2) Setting the value into a DataType (I don't know what to choose yet,
       linkedlist seems the easier solution) and save its information into it
       (Macro Name, Line of file it starts).
     */

    // if (llm_contains(head, token) != -1L){
    //   fprintf(stderr, "Cannot Create Multiple Tokens with the same name\n");
    //   exit(EXIT_FAILURE);
    // }

    // long offset = ftell(in);
    // llm_add(head, token, offset);



    token = strtok(NULL, " \t");
    /* Line Should only have two sets of data, (macro keyword and a name), if it
     * has more it means the line is invalid and execution must stop.*/
    if (*token != '\n') {
      fprintf(stderr, "Macro Initialization has extranous information.");
      exit(EXIT_FAILURE);
    }
    /*TODO: Check if the line is macro name or a keyword, doing two different
     * things for each one */
  }
}
