#include "../include/table.h"
#include "../include/list.h"

#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

typedef struct table_t {
  /* The number of items currently in the table. */
  int count;
  /* List of `table_key_t` - stores the string keys. */
  list_t *key_list;
  /* For each index in the key array, its value is at the same index in this array. */
  list_t *value_list;
} table_t;

result_t table_create(size_t value_size, table_t **table) {
  TRY_MALLOC(table)

  (*table)->count = 0;
  /* We initialize all the lists and tables to NULL so that if their creation fails, they won't have garbage values. */
  (*table)->key_list = NULL;
  (*table)->value_list = NULL;

  TRY(list_create(sizeof(table_key_t), &(*table)->key_list))
  TRY(list_create(value_size, &(*table)->value_list))

  return SUCCESS;
}

size_t table_count(table_t *table) {
  return table->count;
}

result_t table_add(table_t *table, table_key_t key, void **out) {
  /* We make a copy of the parameter, and use that as the key. */
  table_key_t new_key;
  TRY(clone_string(key, &new_key))
  LIST_ADD(table->key_list, new_key)
  TRY(list_add(table->value_list, out))
  table->count++;
  return SUCCESS;
}

void *table_get(table_t *table, table_key_t key) {
  size_t i;
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
  if (!table) {
    return;
  }

  int i;
  /* Free all the key strings. */
  for (i = 0; i < table->count; i++) {
    table_key_t key = *(table_key_t *) list_at(table->key_list, i);
    free(key);
  }
  list_free(table->key_list);

  list_free(table->value_list);

  free(table);
}
