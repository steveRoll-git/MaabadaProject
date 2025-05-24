#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdlib.h>
typedef struct macro_t {
  char *name;
  long index;
} macro_t;

typedef struct llm_t {
  /* If line starts with this character, it means it's  a comment, ignore it
   * and do not parse. */
  // printf("line: %s\n", line);
  macro_t macro;
  struct llm_t *next;
} llm_t;

// llm_t* init_ll(){
//   llm_t* head = (llm_t*) malloc(sizeof(llm_t));
//   return head;
// }
//j


/* adds a new macro into our linkedlist*/
int llm_add(llm_t *head, char* name, long offset);

/*INFO: Do we really need this?*/
/*removes a macro from out offset, */
int llm_rem(llm_t *head, char* name);


/*Checks inside the linked list table for the macro name, if it exists it will return its offset, otherwise it returns -1.*/
long llm_contains(llm_t *head, char* name);

/**/
void print_llm(llm_t* head);

#endif
