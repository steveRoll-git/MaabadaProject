#include "../include/context.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/table.h"
#include "../include/utils.h"

/* The two least significant bits in an instruction word are the "ARE" bits - so the address of the label must be
 * shifted to the left by two bits. */
#define LABEL_FIRST_BIT 2

result_t context_create(char *file_path, table_t *macro_table, context_t **context) {
  TRY_MALLOC(context)

  (*context)->file_path = file_path;
  (*context)->line_number = 1;
  (*context)->ic = CODE_IMAGE_START_ADDRESS;
  (*context)->dc = 0;
  /* We initialize all the lists and tables to NULL so that if their creation fails, they won't have garbage values. */
  (*context)->code_array = NULL;
  (*context)->data_array = NULL;
  (*context)->label_table = NULL;
  (*context)->macro_table = macro_table;
  (*context)->warned_too_large = FALSE;

  TRY(list_create(sizeof(machine_word_t), &(*context)->code_array))
  TRY(list_create(sizeof(machine_word_t), &(*context)->data_array))
  TRY(table_create(sizeof(label_info_t), &(*context)->label_table))

  return SUCCESS;
}

/* Checks that the current size of the program (the data image together with the code image) doesn't exceed the highest
 * address that can be accessed, which is 255. */
/* If the program is too large, an error is returned, but this is only done once for the current file. */
result_t check_max_address(context_t *context) {
  if (!context->warned_too_large && context->ic + context->dc > ADDRESS_MAX) {
    /* If the program is too large, we only show that error once for the current file. */
    context->warned_too_large = TRUE;
    return ERR_PROGRAM_TOO_LARGE;
  }
  return SUCCESS;
}

result_t add_code_word(context_t *context, machine_word_t data) {
  LIST_ADD(context->code_array, data);
  context->ic++;
  TRY(check_max_address(context))
  return SUCCESS;
}

result_t add_data_word(context_t *context, machine_word_t data) {
  LIST_ADD(context->data_array, data);
  context->dc++;
  TRY(check_max_address(context))
  return SUCCESS;
}

/* Initializes a new empty `label_info_t` value for the given label. */
/* May fail if memory allocations did not succeed. */
result_t init_label_info(context_t *assembler, char *label, label_info_t **info) {
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
result_t get_label_info(context_t *assembler, char *label, label_info_t **info) {
  *info = table_get(assembler->label_table, label);
  if (*info == NULL) {
    TRY(init_label_info(assembler, label, info))
  }
  return SUCCESS;
}

result_t add_label_reference(context_t *context, char *label) {
  label_info_t *info;
  label_reference_t *reference;
  TRY(get_label_info(context, label, &info))
  TRY(list_add(info->references, (void **) &reference))

  reference->address = context->ic;
  reference->line_number = context->line_number;

  TRY(add_code_word(context, 0))

  return SUCCESS;
}

result_t add_label(context_t *context, char *label, bool_t is_data) {
  label_info_t *info;
  TRY(get_label_info(context, label, &info))

  /* Make sure that the label wasn't already defined. */
  ASSERT(!info->found, ERR_LABEL_ALREADY_DEFINED)
  /* Make sure that there is no macro with the same name. */
  ASSERT(table_get(context->macro_table, label) == NULL, ERR_LABEL_NAME_IS_MACRO)

  info->found = TRUE;
  info->is_data = is_data;
  info->is_external = FALSE;
  info->value = is_data ? context->dc : context->ic;

  return SUCCESS;
}

result_t add_entry(context_t *context, char *label) {
  label_info_t *info;
  TRY(get_label_info(context, label, &info))
  ASSERT(!info->is_entry, ERR_LABEL_ALREADY_ENTRY)

  info->is_entry = TRUE;
  info->entry_line = context->line_number;

  return SUCCESS;
}

result_t add_extern(context_t *context, char *label) {
  label_info_t *info;
  TRY(get_label_info(context, label, &info))
  /* A label declared with `.extern` cannot be defined more than once in the same file. */
  ASSERT(!info->found, ERR_LABEL_ALREADY_DEFINED)

  info->found = TRUE;
  info->is_external = TRUE;

  return SUCCESS;
}

void merge_data(context_t *context) {
  int i;

  for (i = 0; i < table_count(context->label_table); i++) {
    label_info_t *info = table_value_at(context->label_table, i);
    if (info->found && info->is_data) {
      info->value += context->ic;
    }
  }
}

bool_t resolve_labels(context_t *context) {
  bool_t success = TRUE;
  int i;

  for (i = 0; i < table_count(context->label_table); i++) {
    label_info_t *info = table_value_at(context->label_table, i);

    int j;
    for (j = 0; j < list_count(info->references); j++) {
      label_reference_t *reference = list_at(info->references, j);
      /* The reference's address starts at 100, so we subtract that from it. */
      machine_word_t *word = list_at(context->code_array, reference->address - CODE_IMAGE_START_ADDRESS);

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
        print_error(context->file_path, reference->line_number, ERR_LABEL_NOT_DEFINED);
      }
    }

    if (!info->found && info->is_entry) {
      print_error(context->file_path, info->entry_line, ERR_LABEL_NOT_DEFINED);
    }
  }

  return success;
}

void print_data(context_t *context) {
  int i = 0;
  printf("IC: %d \n", context->ic);
  printf("DC: %d \n", context->dc);

  printf("\nARGS (For DC): { ");

  for (i = 0; i < list_count(context->data_array); i++) {
    printf("%d,  ", *(machine_word_t *) list_at(context->data_array, i));
  }
  printf("}\n");

  printf("Labels:\n");

  for (i = 0; i < table_count(context->label_table); i++) {
    int j;
    label_info_t *info = table_value_at(context->label_table, i);
    printf("%s = %d: ", table_key_at(context->label_table, i), info->value);
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

void context_free(context_t *context) {
  if (!context) {
    return;
  }

  int i;
  list_free(context->code_array);
  list_free(context->data_array);

  if (context->label_table) {
    for (i = 0; i < table_count(context->label_table); i++) {
      label_info_t *info = table_value_at(context->label_table, i);
      label_info_free(info);
    }
    table_free(context->label_table);
  }

  free(context);
}
