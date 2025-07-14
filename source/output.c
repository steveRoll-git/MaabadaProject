#include "../include/output.h"

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

/* Outputs a number in base 4, with `num_digits` digits, into `file`. */
/* If `num_digits` is 0, the number of digits will be as many as needed to represent the number. */
void output_base4(machine_word_t number, int num_digits, FILE *file) {
  int shift;
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
    /* If `num_digits` is 0, we print the digit only if it's not a leading 'a' (zero)/ */
    if (num_digits != 0 || (had_nonzero || shift == 0)) {
      fputc(digit, file);
    }
    shift -= DIGIT_BITS;
  }
}

/* Outputs a base 4 address and a base 4 word into the given file. */
void output_word(machine_word_t address, machine_word_t word, FILE *out) {
  output_base4(address, ADDRESS_DIGITS, out);
  fputc(' ', out);
  output_base4(word, WORD_DIGITS, out);
  fputc('\n', out);
}

/* Outputs the object file (".ob"), containing the assembler's code and data image in base 4 format. */
void output_object(assembler_t *assembler, char *out_path) {
  FILE *out = fopen(out_path, "w");
  machine_word_t address = CODE_IMAGE_START_ADDRESS;
  int i;

  if (out == NULL) {
    printf("Could not open output file \"%s\".\n", out_path);
    exit(EXIT_FAILURE);
  }

  output_base4(list_count(assembler->code_array), 0, out);
  fputc(' ', out);
  output_base4(list_count(assembler->data_array), 0, out);
  fputc('\n', out);

  for (i = 0; i < list_count(assembler->code_array); i++) {
    output_word(address, *(machine_word_t *) list_at(assembler->code_array, i), out);
    address++;
  }

  for (i = 0; i < list_count(assembler->data_array); i++) {
    output_word(address, *(machine_word_t *) list_at(assembler->data_array, i), out);
    address++;
  }

  fclose(out);
}
