#include "../headers/output.h"

#include <stdio.h>
#include <stdlib.h>

/* The numbers we will output are in base 4. */
#define BASE_NUM 4
/* In a 10-bit number, the 9th bit is the most significant bit. */
#define MSB_10 9
/* In a 16-bit number, the 15th bit is the most significant bit. */
#define MSB_16 15
/* Each base 4 digit represents 2 bits in a binary number. */
#define DIGIT_BITS 2
/* Each base 4 digit represents 2 bits in a binary number. 0b11 = 3 */
#define DIGIT_MASK 0x3
/* An address can be at most 255, which is 4 digits in base 4. */
#define ADDRESS_DIGITS 4
/* Machine words are 10 bits wide, which is 5 digits in base 4. */
#define WORD_DIGITS 5

/**
 * Outputs a number in base 4, with `num_digits` digits, into `file`.
 * If `num_digits` is 0, the number of digits will be as many as needed to represent the number without leading 'a's.
 *
 * @param number The number to write.
 * @param num_digits The number of digits to write (with leading 'a's), or 0 to decide automatically.
 * @param file The file to write into.
 */
void output_base4(machine_word_t number, int num_digits, FILE *file) {
  /* The number of bits to shift the number to the left, to get the current base 4 digit. */
  int shift;
  /* Whether we encountered a non-zero ('a') digit. */
  bool_t had_nonzero = FALSE;

  if (num_digits == 0) {
    shift = (WORD_DIGITS - 1) * DIGIT_BITS;
  }
  else {
    shift = (num_digits - 1) * DIGIT_BITS;
  }

  while (shift >= 0) {
    int digit = 'a' + ((number >> shift) & DIGIT_MASK);
    had_nonzero = had_nonzero || digit != 'a';
    /* If the number of digits was given explicitly, we will always print the digit. */
    /* If `num_digits` is 0, we print the digit only if it's not a leading 'a' (zero). */
    if (num_digits != 0 || (had_nonzero || shift == 0)) {
      fputc(digit, file);
    }
    shift -= DIGIT_BITS;
  }
}

/**
 * Outputs a base 4 address and a base 4 word into the given file.
 *
 * @param address The address of the word.
 * @param word The word's value.
 * @param out The file to write into.
 */
void output_word(machine_word_t address, machine_word_t word, FILE *out) {
  output_base4(address, ADDRESS_DIGITS, out);
  fputc(' ', out);
  output_base4(word, WORD_DIGITS, out);
  fputc('\n', out);
}

result_t output_object(context_t *context, char *out_path) {
  machine_word_t address = CODE_IMAGE_START_ADDRESS;
  size_t i;
  FILE *out = fopen(out_path, "w");

  if (out == NULL) {
    return ERR_OUTPUT_FILE_FAIL;
  }

  output_base4(list_count(context->code_array), 0, out);
  fputc(' ', out);
  output_base4(list_count(context->data_array), 0, out);
  fputc('\n', out);

  for (i = 0; i < list_count(context->code_array); i++) {
    output_word(address, *(machine_word_t *) list_at(context->code_array, i), out);
    address++;
  }

  for (i = 0; i < list_count(context->data_array); i++) {
    output_word(address, *(machine_word_t *) list_at(context->data_array, i), out);
    address++;
  }

  fclose(out);

  return SUCCESS;
}

/**
 * Outputs the name of a label along with its address (in base 4) to the given file.
 *
 * @param label The name of the label.
 * @param address The label's address.
 * @param out The file to write to.
 */
void output_label_address(char *label, int address, FILE *out) {
  fputs(label, out);
  fputc(' ', out);
  output_base4(address, ADDRESS_DIGITS, out);
  fputc('\n', out);
}

result_t output_entries_externals(context_t *context, char *entries_path, char *externals_path) {
  result_t result = SUCCESS;
  /* The entries and externals files are only opened if there is data to be written into them. */
  FILE *entries = NULL;
  FILE *externals = NULL;
  size_t i;

  for (i = 0; i < table_count(context->label_table); i++) {
    char *label = table_key_at(context->label_table, i);
    label_info_t *info = table_value_at(context->label_table, i);

    if (info->is_entry) {
      if (!entries) {
        /* Open the entries file only if we have an entry to write into it. */
        /* Return an error if it fails. */
        entries = fopen(entries_path, "w");
        if (!entries) {
          result = ERR_OUTPUT_FILE_FAIL;
          goto cleanup;
        }
      }

      output_label_address(label, info->value, entries);
    }

    if (info->is_external) {
      size_t j;

      for (j = 0; j < list_count(info->references); j++) {
        label_reference_t *reference = list_at(info->references, j);

        if (!externals) {
          /* Open the externals file only if we have an external reference to write into it. */
          /* Return an error if it fails. */
          externals = fopen(externals_path, "w");
          if (!externals) {
            result = ERR_OUTPUT_FILE_FAIL;
            goto cleanup;
          }
        }

        output_label_address(label, reference->address, externals);
      }
    }
  }

cleanup:
  if (entries) {
    fclose(entries);
  }
  if (externals) {
    fclose(externals);
  }

  return result;
}
