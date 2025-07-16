#ifndef ERRORS_H
#define ERRORS_H

/* Any function that can succeed or fail returns this type. If the function succeeds, it returns 0, otherwise it returns
 * a pointer to the string that represents the error. */
typedef char *result_t;

/* Functions that return a `result_t` should return this if they ran successfully. */
#define SUCCESS 0

/* Instructions */

#define ERR_UNKNOWN_INSTRUCTION "Unknown instruction."
#define ERR_UNKNOWN_DIRECTIVE "Unknown directive."
#define ERR_NOT_ENOUGH_ARGS "Instruction did not receive enough arguments."
#define ERR_TOO_MANY_ARGS "Instruction received too many arguments."
#define ERR_SRC_IMMEDIATE "This instruction doesn't support immediate source operands."
#define ERR_SRC_REGISTER "This instruction doesn't support register source operands."
#define ERR_DST_IMMEDIATE "This instruction doesn't support immediate destination operands."

/* Arguments */

#define ERR_NO_ARG "Expected an argument."
#define ERR_EXTRANEOUS_TEXT_INSTRUCTION "Extraneous text after instruction."
#define ERR_EXTRANEOUS_TEXT_DIRECTIVE "Extraneous text after directive."
#define ERR_INVALID_ARGUMENT "Invalid argument syntax."
#define ERR_DIRECTIVE_EXPECTED_LABEL "This directive expects a label as a parameter."

/* Numbers */

#define ERR_NUMBER_NOT_VALID "Invalid number."
#define ERR_NUMBER_AFTER_HASH "A hash must immediately be followed by a number."

/* Labels */

#define ERR_INVALID_LABEL "Invalid label name: Must not be a reserved word."
#define ERR_LABEL_UNDERSCORES "Label name cannot contain underscores."
#define ERR_LABEL_TOO_LONG "Label name cannot be longer than 30 characters."
#define ERR_LABEL_ALREADY_DEFINED "This label has already been defined in this file."
#define ERR_LABEL_NAME_IS_MACRO "The name of this label is already used as a macro."
#define ERR_LABEL_NOT_DEFINED "This label was not defined."
#define ERR_LABEL_ALREADY_ENTRY "This label was already defined as an entry."
#define ERR_LABEL_MISSING_SPACE "There must be a space after the `:` of a label."

/* Matrix */

#define ERR_MATRIX_OVERFLOW "Matrix has more values than it can store."
#define ERR_MATRIX_START_BRACKET_ROW "Missing '[' for row index."
#define ERR_MATRIX_END_BRACKET_ROW "Missing ']' for row index."
#define ERR_MATRIX_START_BRACKET_COL "Missing '[' for column index."
#define ERR_MATRIX_END_BRACKET_COL "Missing ']' for column index."
#define ERR_MATRIX_NEGATIVE_STORAGE "Matrix rows and colums must be positive integers."
#define ERR_WRONG_MATRIX_ACCESS "Matrix rows and columns can only be accessed with registers."

/* String */

#define ERR_STRING_MISSING_QUOTE_START "String argument must start with quotation marks."
#define ERR_STRING_MISSING_QUOTE_END "String argument must end with quotation marks."

#define ERR_INVALID_MACRO_NAME "Macro definition doesn't contain a valid name."
#define ERR_EXTRANEOUS_TEXT_MACRO "Extraneous text after macro definition."
#define ERR_EXTRANEOUS_TEXT_MCROEND "Extraneous text after `mcroend`."
#define ERR_LINE_TOO_LONG "Line is longer than 80 characters."
#define ERR_FSEEK_FAILED "fseek failed to run."
#define ERR_PREPROCESS_FAILED "One or more errors were encountered during preprocessing."
#define ERR_CODEGEN_FAILED "One or more errors were encountered during code generation."
#define ERR_INPUT_FILE_FAIL "Couldn't open input file."
#define ERR_OUTPUT_FILE_FAIL "Couldn't open output file."
#define ERR_OUT_OF_MEMORY "Out of memory."

/* Checks that the condition is true. If not, causes the running function to return the given error message as a
 * `result_t`. */
#define ASSERT(a, s)                                                                                                   \
  if (!(a)) {                                                                                                          \
    return (result_t) s;                                                                                               \
  }

/* The given expression should be one of `result_t` type. If it returns an error string, causes the running function to
 * return that same error string. */
#define TRY(a)                                                                                                         \
  do {                                                                                                                 \
    result_t _result = (a);                                                                                            \
    if (_result != SUCCESS) {                                                                                          \
      return _result;                                                                                                  \
    }                                                                                                                  \
  }                                                                                                                    \
  while (0);

/* Prints an error message. */
void print_error(char *file_path, int line_number, char *message);

#endif
