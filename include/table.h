#ifndef TABLE_H
#define TABLE_H

#include <stddef.h>

/* The type of the table's keys. */
typedef char *table_key_t;

/* A table associates strings with values. It can store and retrieve values based on a string key. */
typedef struct table_t table_t;

/* Creates a new table where the size of values is the given parameter. */
table_t *table_create(size_t value_size);

/* Returns the number of values in the table. */
size_t table_count(table_t *table);

/* Adds a key to the table, and returns a pointer at which the value for this key should be stored. */
void *table_add(table_t *table, table_key_t key);

/* Assumes that the table was created with the same value size as the given value's size. */
/* Uses table_add to add the given key to the table, and stores the given value there. */
#define TABLE_ADD(table, key, value) *((typeof(value) *) table_add((table), (key))) = (value);

/* Finds the value that's associated with the given string. */
/* Returns a pointer to that value if it exists, or NULL otherwise. */
void *table_get(table_t *table, table_key_t key);

/* Returns a pointer to the key at the given index. */
table_key_t table_key_at(table_t *table, int index);

/* Returns a pointer to the value at the given index. */
void *table_value_at(table_t *table, int index);

/* Frees the memory used by this table. */
/* (This does not free any pointers that the table's values may have.) */
void table_free(table_t *table);

#endif
