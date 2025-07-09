#include "table.h"

#include <stdlib.h>
#include <string.h>

typedef struct table_t {
  /* This array stores the string keys. */
  list_t *key_list;
  /* For each index in the key array, its value is at the same index in this array. */
  list_t *value_list;
} table_t;

table_t *table_create(size_t value_size) {
  table_t *table = malloc(sizeof(table_t));
  table->key_list = list_create(sizeof(char *));
  table->value_list = list_create(value_size);
  return table;
}

size_t table_count(table_t *table) {
  return list_count(table->value_list);
}

void *table_add(table_t *table, table_key_t key) {
  /* We make a copy of the parameter, and use that as the key. */
  table_key_t new_key = malloc(sizeof(char) * strlen(key));
  strcpy(new_key, key);
  LIST_ADD(table->key_list, new_key)

  return list_add(table->value_list);
}

void *table_get(table_t *table, table_key_t key) {
  int i;
  for (i = 0; i < list_count(table->key_list); i++) {
    table_key_t current_key = table_key_at(table, i);
    if (strcmp(key, current_key) == 0) {
      return list_at(table->value_list, i);
    }
  }
  return NULL;
}

table_key_t table_key_at(table_t *table, int index) {
  return *(table_key_t *) list_at(table->key_list, index);
}

void *table_value_at(table_t *table, int index) {
  return list_at(table->value_list, index);
}

void table_free(table_t *table) {
  int i;
  /* Free all the key strings. */
  for (i = 0; i < list_count(table->key_list); i++) {
    table_key_t key = *(table_key_t *) list_at(table->key_list, i);
    free(key);
  }
  list_free(table->key_list);

  list_free(table->value_list);

  free(table);
}
