#ifndef ERRORS_H
#define ERRORS_H

/* Any function that can succeed or fail returns this type. */
/* If the function succeeds, it returns 0, otherwise it returns the relevant error code. */
typedef enum result_t {
  /* Returned when no error occurred. */
  SUCCESS = 0,

  /* Generic assembler errors */

  ERR_PROGRAM_TOO_LARGE,
  ERR_PREPROCESS_FAILED,
  ERR_CODEGEN_FAILED,
  ERR_UNDEFINED_LABELS,
  ERR_INPUT_FILE_FAIL,
  ERR_OUTPUT_FILE_FAIL,
  ERR_OUT_OF_MEMORY,

  /* Preprocessor errors */

  ERR_INVALID_MACRO_NAME,
  ERR_EXTRANEOUS_TEXT_MACRO,
  ERR_EXTRANEOUS_TEXT_MCROEND,
  ERR_LINE_TOO_LONG,
  ERR_FSEEK_FAILED,

  /* Instruction syntax errors */

  ERR_UNKNOWN_INSTRUCTION,
  ERR_UNKNOWN_DIRECTIVE,
  ERR_NOT_ENOUGH_ARGS,
  ERR_TOO_MANY_ARGS,
  ERR_SRC_IMMEDIATE,
  ERR_SRC_REGISTER,
  ERR_DST_IMMEDIATE,

  /* Argument syntax errors */

  ERR_NO_ARG,
  ERR_EXTRANEOUS_TEXT_INSTRUCTION,
  ERR_EXTRANEOUS_TEXT_DIRECTIVE,
  ERR_INVALID_ARGUMENT,
  ERR_INVALID_DIRECTIVE_SYNTAX,
  ERR_DIRECTIVE_EXPECTED_LABEL,

  /* Number syntax errors */

  ERR_NUMBER_NOT_VALID,
  ERR_NUMBER_AFTER_HASH,
  ERR_IMMEDIATE_TOO_SMALL,
  ERR_IMMEDIATE_TOO_LARGE,
  ERR_WORD_TOO_SMALL,
  ERR_WORD_TOO_LARGE,

  /* Label syntax errors */

  ERR_INVALID_LABEL,
  ERR_LABEL_UNDERSCORES,
  ERR_LABEL_TOO_LONG,
  ERR_LABEL_ALREADY_DEFINED,
  ERR_LABEL_NAME_IS_MACRO,
  ERR_LABEL_NOT_DEFINED,
  ERR_LABEL_ALREADY_ENTRY,
  ERR_LABEL_MISSING_SPACE,

  /* Matrix syntax errors */

  ERR_MATRIX_OVERFLOW,
  ERR_MATRIX_START_BRACKET_ROW,
  ERR_MATRIX_END_BRACKET_ROW,
  ERR_MATRIX_START_BRACKET_COL,
  ERR_MATRIX_END_BRACKET_COL,
  ERR_MATRIX_NEGATIVE_STORAGE,
  ERR_WRONG_MATRIX_ACCESS,
  ERR_MISSING_SPACE_AFTER_BRACKET,

  /* String syntax errors */

  ERR_STRING_MISSING_QUOTE_START,
  ERR_STRING_MISSING_QUOTE_END
} result_t;

/* A warning that may be reported in a file. */
typedef enum warning_t {
  WARNING_ENTRY_LABEL,
  WARNING_EXTERN_LABEL
} warning_t;

/**
 * This macro can only be called in a function that returns a `result_t`.
 * Checks that the condition is true. If not, causes the running function to return the given error code.
 *
 * @param cond The condition to check.
 * @param error The error to return if the condition is false.
 */
#define ASSERT(cond, error)                                                                                            \
  if (!(cond)) {                                                                                                       \
    return error;                                                                                                      \
  }

/**
 * This macro can only be called in a function that returns a `result_t`.
 * Evaluates the given expression, which must also return a `result_t`. If the returned result is not `SUCCESS`, causes
 * the running function to return that same error. Otherwise, continues normally.
 *
 * @param exp The expression to try. Must return a `result_t`.
 */
#define TRY(exp)                                                                                                       \
  {                                                                                                                    \
    result_t _result = (exp);                                                                                          \
    if (_result != SUCCESS) {                                                                                          \
      return _result;                                                                                                  \
    }                                                                                                                  \
  }

/**
 * Prints an error message that corresponds to the given error code.
 *
 * @param result The error code to print the message for.
 */
void print_error(result_t result);

/**
 * Prints an error message that corresponds to the given error code, with a file path and line number.
 *
 * @param file_path The path of the file where the error was found.
 * @param line_number The number of the line where the error was found.
 * @param result The result whose message should be printed.
 */
void print_error_in_file(char *file_path, int line_number, result_t result);

/**
 * Prints a warning message that corresponds to the given warning code, with a file path and line number.
 *
 * @param file_path The path of the file where the warning was found.
 * @param line_number The number of the line where the warning was found.
 * @param warning A message describing the warning.
 */
void print_warning_in_file(char *file_path, int line_number, warning_t warning);

#endif
