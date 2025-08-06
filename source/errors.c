#include "../headers/errors.h"

#include <stdio.h>

/**
 * Given a `result_t`, returns the error message that describes it.
 *
 * @param result The `result_t` to get the message for.
 * @return An error message that describes the result.
 */
const char *get_error_message(result_t result) {
  switch (result) {
    case ERR_PROGRAM_TOO_LARGE:
      return "The program is too large after this point (address exceeds 255).";
    case ERR_PREPROCESS_FAILED:
      return "One or more errors were encountered during preprocessing.";
    case ERR_CODEGEN_FAILED:
      return "One or more errors were encountered during code generation.";
    case ERR_UNDEFINED_LABELS:
      return "One or more labels were not defined.";
    case ERR_INPUT_FILE_FAIL:
      return "Couldn't open input file.";
    case ERR_OUTPUT_FILE_FAIL:
      return "Couldn't open output file.";
    case ERR_OUT_OF_MEMORY:
      return "Out of memory.";
    case ERR_INVALID_MACRO_NAME:
      return "Macro definition doesn't contain a valid name.";
    case ERR_EXTRANEOUS_TEXT_MACRO:
      return "Extraneous text after macro definition.";
    case ERR_EXTRANEOUS_TEXT_MCROEND:
      return "Extraneous text after `mcroend`.";
    case ERR_LINE_TOO_LONG:
      return "Line is longer than 80 characters.";
    case ERR_FSEEK_FAILED:
      return "fseek failed to run.";
    case ERR_UNKNOWN_INSTRUCTION:
      return "Unknown instruction.";
    case ERR_UNKNOWN_DIRECTIVE:
      return "Unknown directive.";
    case ERR_NOT_ENOUGH_ARGS:
      return "Instruction did not receive enough arguments.";
    case ERR_TOO_MANY_ARGS:
      return "Instruction received too many arguments.";
    case ERR_SRC_IMMEDIATE:
      return "This instruction doesn't support immediate source operands.";
    case ERR_SRC_REGISTER:
      return "This instruction doesn't support register source operands.";
    case ERR_DST_IMMEDIATE:
      return "This instruction doesn't support immediate destination operands.";
    case ERR_NO_ARG:
      return "Expected an argument.";
    case ERR_EXTRANEOUS_TEXT_INSTRUCTION:
      return "Extraneous text after instruction.";
    case ERR_EXTRANEOUS_TEXT_DIRECTIVE:
      return "Extraneous text after directive.";
    case ERR_INVALID_ARGUMENT:
      return "Invalid argument syntax.";
    case ERR_INVALID_DIRECTIVE_SYNTAX:
      return "Invalid directive syntax.";
    case ERR_DIRECTIVE_EXPECTED_LABEL:
      return "This directive expects a label as a parameter.";
    case ERR_NUMBER_NOT_VALID:
      return "Invalid number.";
    case ERR_NUMBER_AFTER_HASH:
      return "A hash must immediately be followed by a number.";
    case ERR_IMMEDIATE_TOO_SMALL:
      return "The immediate operand is smaller than the minimum possible value (-128).";
    case ERR_IMMEDIATE_TOO_LARGE:
      return "The immediate operand is larger than the maximum possible value (127).";
    case ERR_WORD_TOO_SMALL:
      return "The number is smaller than the minimum possible word (-512).";
    case ERR_WORD_TOO_LARGE:
      return "The number is larger than the maximum possible word (511).";
    case ERR_INVALID_LABEL:
      return "Invalid label name: Must not be a reserved word.";
    case ERR_LABEL_UNDERSCORES:
      return "Label name cannot contain underscores.";
    case ERR_LABEL_TOO_LONG:
      return "Label name cannot be longer than 30 characters.";
    case ERR_LABEL_ALREADY_DEFINED:
      return "This label has already been defined in this file.";
    case ERR_LABEL_NAME_IS_MACRO:
      return "The name of this label is already used as a macro.";
    case ERR_LABEL_NOT_DEFINED:
      return "This label was not defined.";
    case ERR_LABEL_ALREADY_ENTRY:
      return "This label was already defined as an entry.";
    case ERR_LABEL_MISSING_SPACE:
      return "There must be a space after the `:` of a label.";
    case ERR_MATRIX_OVERFLOW:
      return "Matrix has more values than it can store.";
    case ERR_MATRIX_START_BRACKET_ROW:
      return "Missing '[' for row index.";
    case ERR_MATRIX_END_BRACKET_ROW:
      return "Missing ']' for row index.";
    case ERR_MATRIX_START_BRACKET_COL:
      return "Missing '[' for column index.";
    case ERR_MATRIX_END_BRACKET_COL:
      return "Missing ']' for column index.";
    case ERR_MATRIX_NEGATIVE_STORAGE:
      return "Matrix rows and columns must be positive integers.";
    case ERR_WRONG_MATRIX_ACCESS:
      return "Matrix rows and columns can only be accessed with registers.";
    case ERR_MISSING_SPACE_AFTER_BRACKET:
      return "Missing space after last `]`.";
    case ERR_STRING_MISSING_QUOTE_START:
      return "String argument must start with quotation marks.";
    case ERR_STRING_MISSING_QUOTE_END:
      return "String argument must end with quotation marks.";
    default:
      return "Unknown error code.";
  }
}

void print_error(result_t result) {
  printf("%s", get_error_message(result));
}

void print_error_in_file(char *file_path, int line_number, result_t result) {
  printf("Error at %s:%d: %s\n", file_path, line_number, get_error_message(result));
}

/**
 * Returns the warning message that corresponds to the given warning code.
 *
 * @param warning The warning to get the message for.
 * @return A message describing the warning.
 */
const char *get_warning_message(warning_t warning) {
  switch (warning) {
    case WARNING_ENTRY_LABEL:
      return "Labels on '.entry' directives have no effect and are ignored.";
    case WARNING_EXTERN_LABEL:
      return "Labels on '.extern' directives have no effect and are ignored.";
    default:
      return "Unknown warning code.";
  }
}

void print_warning_in_file(char *file_path, int line_number, warning_t warning) {
  printf("Warning at %s:%d: %s\n", file_path, line_number, get_warning_message(warning));
}
