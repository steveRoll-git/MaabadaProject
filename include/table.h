#ifndef TABLE_H
#define TABLE_H

#include <stddef.h>

#include "errors.h"

/* The type of the table's keys. */
typedef char *table_key_t;

/* A table associates strings with values. It can store and retrieve values based on a string key. */
typedef struct table_t table_t;

/* Creates a new table with the specified value size, and stores it in `table`. */
/* May fail if memory allocations did not succeed. */
result_t table_create(size_t value_size, table_t **table);

/* Returns the number of values in the table. */
size_t table_count(table_t *table);

/* Adds a key to the table, and returns a pointer at which the value for this key should be stored. */
/* May fail if memory allocations did not succeed. */
result_t table_add(table_t *table, table_key_t key, void **out);

/* Assumes that the table was created with the same value size as the given value's size. */
/* Uses table_add to add the given key to the table, and stores the given value there. */
/* table_add may fail, so this must be called in a function that returns a `result_t`. */
#define TABLE_ADD(table, key, value)                                                                                   \
  do {                                                                                                                 \
    void *_ptr;                                                                                                        \
    TRY(table_add(table, key, &_ptr))                                                                                  \
    *((typeof(value) *) _ptr) = (value);                                                                               \
  }                                                                                                                    \
  while (0);

/* Finds the value that's associated with the given string. */
/* Returns a pointer to that value if it exists, or NULL otherwise. */
void *table_get(table_t *table, table_key_t key);

/* Returns the key at the given index. */
table_key_t table_key_at(table_t *table, int index);

/* Returns a pointer to the value at the given index. */
void *table_value_at(table_t *table, int index);

/* Frees the memory used by this table, including the memory used by the keys. */
/* (This does not free any pointers that the table's values may have.) */
/* If the given pointer is NULL, nothing is performed. */
void table_free(table_t *table);

#endif
