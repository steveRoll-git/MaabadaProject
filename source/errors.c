#include "../include/errors.h"

#include <stdio.h>

/* Generic assembler errors */

result_t ERR_PREPROCESS_FAILED = "One or more errors were encountered during preprocessing.";
result_t ERR_CODEGEN_FAILED = "One or more errors were encountered during code generation.";
result_t ERR_INPUT_FILE_FAIL = "Couldn't open input file.";
result_t ERR_OUTPUT_FILE_FAIL = "Couldn't open output file.";
result_t ERR_OUT_OF_MEMORY = "Out of memory.";

/* Preprocessor errors */

result_t ERR_INVALID_MACRO_NAME = "Macro definition doesn't contain a valid name.";
result_t ERR_EXTRANEOUS_TEXT_MACRO = "Extraneous text after macro definition.";
result_t ERR_EXTRANEOUS_TEXT_MCROEND = "Extraneous text after `mcroend`.";
result_t ERR_LINE_TOO_LONG = "Line is longer than 80 characters.";
result_t ERR_FSEEK_FAILED = "fseek failed to run.";

/* Instruction syntax errors */

result_t ERR_UNKNOWN_INSTRUCTION = "Unknown instruction.";
result_t ERR_UNKNOWN_DIRECTIVE = "Unknown directive.";
result_t ERR_NOT_ENOUGH_ARGS = "Instruction did not receive enough arguments.";
result_t ERR_TOO_MANY_ARGS = "Instruction received too many arguments.";
result_t ERR_SRC_IMMEDIATE = "This instruction doesn't support immediate source operands.";
result_t ERR_SRC_REGISTER = "This instruction doesn't support register source operands.";
result_t ERR_DST_IMMEDIATE = "This instruction doesn't support immediate destination operands.";

/* Argument syntax errors */

result_t ERR_NO_ARG = "Expected an argument.";
result_t ERR_EXTRANEOUS_TEXT_INSTRUCTION = "Extraneous text after instruction.";
result_t ERR_EXTRANEOUS_TEXT_DIRECTIVE = "Extraneous text after directive.";
result_t ERR_INVALID_ARGUMENT = "Invalid argument syntax.";
result_t ERR_DIRECTIVE_EXPECTED_LABEL = "This directive expects a label as a parameter.";

/* Number syntax errors */

result_t ERR_NUMBER_NOT_VALID = "Invalid number.";
result_t ERR_NUMBER_AFTER_HASH = "A hash must immediately be followed by a number.";
result_t ERR_IMMEDIATE_TOO_SMALL = "The immediate operand is smaller than the minimum possible value (-128).";
result_t ERR_IMMEDIATE_TOO_LARGE = "The immediate operand is larger than the maximum possible value (127).";

/* Label syntax errors */

result_t ERR_INVALID_LABEL = "Invalid label name: Must not be a reserved word.";
result_t ERR_LABEL_UNDERSCORES = "Label name cannot contain underscores.";
result_t ERR_LABEL_TOO_LONG = "Label name cannot be longer than 30 characters.";
result_t ERR_LABEL_ALREADY_DEFINED = "This label has already been defined in this file.";
result_t ERR_LABEL_NAME_IS_MACRO = "The name of this label is already used as a macro.";
result_t ERR_LABEL_NOT_DEFINED = "This label was not defined.";
result_t ERR_LABEL_ALREADY_ENTRY = "This label was already defined as an entry.";
result_t ERR_LABEL_MISSING_SPACE = "There must be a space after the `:` of a label.";

/* Matrix syntax errors */

result_t ERR_MATRIX_OVERFLOW = "Matrix has more values than it can store.";
result_t ERR_MATRIX_START_BRACKET_ROW = "Missing '[' for row index.";
result_t ERR_MATRIX_END_BRACKET_ROW = "Missing ']' for row index.";
result_t ERR_MATRIX_START_BRACKET_COL = "Missing '[' for column index.";
result_t ERR_MATRIX_END_BRACKET_COL = "Missing ']' for column index.";
result_t ERR_MATRIX_NEGATIVE_STORAGE = "Matrix rows and columns must be positive integers.";
result_t ERR_WRONG_MATRIX_ACCESS = "Matrix rows and columns can only be accessed with registers.";

/* String syntax errors */

result_t ERR_STRING_MISSING_QUOTE_START = "String argument must start with quotation marks.";
result_t ERR_STRING_MISSING_QUOTE_END = "String argument must end with quotation marks.";

void print_error(char *file_path, int line_number, const char *message) {
  printf("Error at %s:%d: %s\n", file_path, line_number, message);
}
