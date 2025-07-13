#ifndef UTILS_H
#define UTILS_H

/* Allocates memory and creates a copy of the given string. */
/* The returned pointer must be freed when it is no longer needed. */
char *clone_string(char *orig);

/* Creates a new string whose value is strings `a` and `b` joined together. */
/* The returned pointer must be freed when it is no longer needed. */
char *join_strings(char *a, char *b);

#endif
