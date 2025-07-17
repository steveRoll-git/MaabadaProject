#ifndef TABLE_H
#define TABLE_H

#include <stddef.h>

#include "errors.h"
#include "list.h"

/* The type of the table's keys. */
typedef char *table_key_t;

/* A table associates strings with values. It can store and retrieve values based on a string key. */
typedef struct table_t {
  /* The number of items currently in the table. */
  int count;
  /* List of `table_key_t` - stores the string keys. */
  list_t *key_list;
  /* For each index in the key array, its value is at the same index in this array. */
  list_t *value_list;
} table_t;

/* Creates a new table with the specified value size, and stores it in `table`. */
/* May fail if memory allocations did not succeed. */
result_t table_create(size_t value_size, table_t **table);

/* Returns the number of values in the table. */
size_t table_count(table_t *table);

/* Adds a key to the table, and returns a pointer at which the value for this key should be stored. */
/* May fail if memory allocations did not succeed. */
result_t table_add(table_t *table, table_key_t key, void **out);

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
