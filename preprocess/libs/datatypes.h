#ifndef DATATYPES_H
#define DATATYPES_H

struct macro_t {
  char *name;
  long index;
};

struct llm_t {
  macro_t macro;
  llm_t *next;
};

#endif
