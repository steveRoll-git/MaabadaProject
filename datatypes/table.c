#include "table.h"

#include <stdlib.h>
#include <string.h>

typedef struct table_t {
  /* This array stores the string keys. */
  array_t *key_array;
  /* For each index in the key array, its value is at the same index in this array. */
  array_t *value_array;
} table_t;

table_t *table_create(size_t value_size) {
  table_t *table = malloc(sizeof(table_t));
  table->key_array = array_create(sizeof(char *));
  table->value_array = array_create(value_size);
  return table;
}

void *table_add(table_t *table, table_key_t key) {
  /* We make a copy of the parameter, and use that as the key. */
  table_key_t new_key = malloc(sizeof(char) * strlen(key));
  strcpy(new_key, key);
  ARRAY_ADD(table->key_array, new_key)

  return array_add(table->value_array);
}

void *table_get(table_t *table, table_key_t key) {
  int i;
  for (i = 0; i < array_count(table->key_array); i++) {
    table_key_t current_key = *(table_key_t *) array_at(table->key_array, i);
    if (strcmp(key, current_key) == 0) {
      return array_at(table->value_array, i);
    }
  }
  return NULL;
}

void table_free(table_t *table) {
  int i;
  /* Free all the key strings. */
  for (i = 0; i < array_count(table->key_array); i++) {
    table_key_t key = *(table_key_t *) array_at(table->key_array, i);
    free(key);
  }
  array_free(table->key_array);

  array_free(table->value_array);

  free(table);
}
