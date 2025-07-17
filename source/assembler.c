#include "../include/assembler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/table.h"
#include "../include/utils.h"

/* The two least significant bits in an instruction word are the "ARE" bits - so the address of the label must be
 * shifted to the left by two bits. */
#define LABEL_FIRST_BIT 2

result_t assembler_create(char *file_path, table_t *macro_table, assembler_t **assembler) {
  TRY_MALLOC(assembler)

  (*assembler)->file_path = file_path;
  (*assembler)->line_number = 1;
  (*assembler)->ic = CODE_IMAGE_START_ADDRESS;
  (*assembler)->dc = 0;
  /* We initialize all the lists and tables to NULL so that if their creation fails, they won't have garbage values. */
  (*assembler)->code_array = NULL;
  (*assembler)->data_array = NULL;
  (*assembler)->label_table = NULL;
  (*assembler)->macro_table = macro_table;

  TRY(list_create(sizeof(machine_word_t), &(*assembler)->code_array))
  TRY(list_create(sizeof(machine_word_t), &(*assembler)->data_array))
  TRY(table_create(sizeof(label_info_t), &(*assembler)->label_table))

  return SUCCESS;
}

result_t add_code_word(assembler_t *assembler, machine_word_t data) {
  LIST_ADD(assembler->code_array, data);
  assembler->ic++;
  return SUCCESS;
}

result_t add_data_word(assembler_t *assembler, machine_word_t data) {
  LIST_ADD(assembler->data_array, data);
  assembler->dc++;
  return SUCCESS;
}

/* Initializes a new empty `label_info_t` value for the given label. */
/* May fail if memory allocations did not succeed. */
result_t init_label_info(assembler_t *assembler, char *label, label_info_t **info) {
  TRY(table_add(assembler->label_table, label, (void **) info))
  (*info)->found = FALSE;
  (*info)->is_data = FALSE;
  (*info)->is_external = FALSE;
  (*info)->is_entry = FALSE;
  (*info)->references = NULL;
  TRY(list_create(sizeof(label_reference_t), &(*info)->references))
  return SUCCESS;
}

/* Returns a pointer to the `label_info_t` object for the specified label. */
/* If an info object doesn't exist for this label yet, creates it. */
/* May fail if memory allocations did not succeed. */
result_t get_label_info(assembler_t *assembler, char *label, label_info_t **info) {
  *info = table_get(assembler->label_table, label);
  if (*info == NULL) {
    TRY(init_label_info(assembler, label, info))
  }
  return SUCCESS;
}

result_t add_label_reference(assembler_t *assembler, char *label) {
  label_info_t *info;
  label_reference_t *reference;
  TRY(get_label_info(assembler, label, &info))
  TRY(list_add(info->references, (void **) &reference))

  reference->address = assembler->ic;
  reference->line_number = assembler->line_number;

  TRY(add_code_word(assembler, 0))

  return SUCCESS;
}

result_t add_label(assembler_t *assembler, char *label, bool_t is_data) {
  label_info_t *info;
  TRY(get_label_info(assembler, label, &info))

  /* Make sure that the label wasn't already defined. */
  ASSERT(!info->found, ERR_LABEL_ALREADY_DEFINED)
  /* Make sure that there is no macro with the same name. */
  ASSERT(table_get(assembler->macro_table, label) == NULL, ERR_LABEL_NAME_IS_MACRO)

  info->found = TRUE;
  info->is_data = is_data;
  info->is_external = FALSE;
  info->value = is_data ? assembler->dc : assembler->ic;

  return SUCCESS;
}

result_t add_entry(assembler_t *assembler, char *label) {
  label_info_t *info;
  TRY(get_label_info(assembler, label, &info))
  ASSERT(!info->is_entry, ERR_LABEL_ALREADY_ENTRY)

  info->is_entry = TRUE;
  info->entry_line = assembler->line_number;

  return SUCCESS;
}

result_t add_extern(assembler_t *assembler, char *label) {
  label_info_t *info;
  TRY(get_label_info(assembler, label, &info))
  /* A label declared with `.extern` cannot be defined more than once in the same file. */
  ASSERT(!info->found, ERR_LABEL_ALREADY_DEFINED)

  info->found = TRUE;
  info->is_external = TRUE;

  return SUCCESS;
}

void merge_data(assembler_t *assembler) {
  int i;

  for (i = 0; i < table_count(assembler->label_table); i++) {
    label_info_t *info = table_value_at(assembler->label_table, i);
    if (info->found && info->is_data) {
      info->value += assembler->ic;
    }
  }
}

bool_t resolve_labels(assembler_t *assembler) {
  bool_t success = TRUE;
  int i;

  for (i = 0; i < table_count(assembler->label_table); i++) {
    label_info_t *info = table_value_at(assembler->label_table, i);

    int j;
    for (j = 0; j < list_count(info->references); j++) {
      label_reference_t *reference = list_at(info->references, j);
      /* The reference's address starts at 100, so we subtract that from it. */
      machine_word_t *word = list_at(assembler->code_array, reference->address - CODE_IMAGE_START_ADDRESS);

      if (info->is_external) {
        /* External labels are all zeroes, with the "ARE" bits being "01", meaning external encoding. */
        *word = ENCODING_EXTERNAL;
      }
      else if (info->found) {
        /* Labels in this file start at bit 2, with the "ARE" bits being "10", meaning relocatable encoding. */
        *word = (info->value << LABEL_FIRST_BIT) | ENCODING_RELOCATABLE;
      }
      else {
        success = FALSE;
        print_error(assembler->file_path, reference->line_number, ERR_LABEL_NOT_DEFINED);
      }
    }

    if (!info->found && info->is_entry) {
      print_error(assembler->file_path, info->entry_line, ERR_LABEL_NOT_DEFINED);
    }
  }

  return success;
}

void print_data(assembler_t *assembler) {
  int i = 0;
  printf("IC: %d \n", assembler->ic);
  printf("DC: %d \n", assembler->dc);

  printf("\nARGS (For DC): { ");

  for (i = 0; i < list_count(assembler->data_array); i++) {
    printf("%d,  ", *(machine_word_t *) list_at(assembler->data_array, i));
  }
  printf("}\n");

  printf("Labels:\n");

  for (i = 0; i < table_count(assembler->label_table); i++) {
    int j;
    label_info_t *info = table_value_at(assembler->label_table, i);
    printf("%s = %d: ", table_key_at(assembler->label_table, i), info->value);
    for (j = 0; j < list_count(info->references); j++) {
      printf("%d, ", ((label_reference_t *) list_at(info->references, j))->line_number);
    }
    printf("\n");
  }
}

/* Frees the memory of a `label_info_t` object. */
void label_info_free(label_info_t *info) {
  list_free(info->references);
}

void assembler_free(assembler_t *assembler) {
  if (!assembler) {
    return;
  }

  int i;
  list_free(assembler->code_array);
  list_free(assembler->data_array);

  if (assembler->label_table) {
    for (i = 0; i < table_count(assembler->label_table); i++) {
      label_info_t *info = table_value_at(assembler->label_table, i);
      label_info_free(info);
    }
    table_free(assembler->label_table);
  }

  free(assembler);
}
