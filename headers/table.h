/*
 * table.h
 *
 * This file defines the "table" data structure. A table associates values of arbitrary size to a string key, and allows
 * looking up values that match a given key. It does this by storing two lists - one for the keys, and another for the
 * values. When an item is added, the index of its key in the key list corresponds to the index of its value in the
 * value list.
 * (This data structure is not a hash table - looking up items is an O(n) operation.)
 */

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

/**
 * Creates a new table with the specified value size, and stores a pointer to it in `table`.
 * May fail if memory allocations did not succeed.
 *
 * @param value_size The size of the value type that the table will store, in bytes.
 * @param table A pointer to the pointer where the created table will be stored.
 * @return The operation's result.
 */
result_t table_create(size_t value_size, table_t **table);

/**
 * Returns the number of values in the table.
 *
 * @param table The table to check.
 * @return The number of values in the table.
 */
size_t table_count(table_t *table);

/**
 * Adds a key to the table, and stores the pointer at which the value for this key should be stored in `out`.
 * May fail if memory allocations did not succeed.
 *
 * @param table The table to add into.
 * @param key The key of the new value.
 * @param out A pointer where the pointer to the new value will be stored.
 * @return The operation's result.
 */
result_t table_add(table_t *table, table_key_t key, void **out);

/**
 * Finds the value that's associated with the given string.
 *
 * @param table The table to look in.
 * @param key The key to find the value of.
 * @return A pointer to the value if the key exists, or NULL otherwise.
 */
void *table_get(table_t *table, table_key_t key);

/**
 * Returns the key at the given index.
 *
 * @param table The table to look in.
 * @param index The index to return the key at.
 * @return The key at the given index.
 */
table_key_t table_key_at(table_t *table, int index);

/**
 * Returns a pointer to the value at the given index.
 *
 * @param table The table to look in.
 * @param index The index to return the value at.
 * @return The pointer to the value at the given index.
 */
void *table_value_at(table_t *table, int index);

/**
 * Frees the memory used by this table, including the memory used by the keys.
 * (This does not free any pointers that the table's values may have.)
 * If the given pointer is NULL, nothing is performed.
 *
 * @param table The table to free. May be NULL.
 */
void table_free(table_t *table);

#endif
