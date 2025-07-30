#ifndef ERRORS_H
#define ERRORS_H

/* Any function that can succeed or fail returns this type. If the function succeeds, it returns 0, otherwise it returns
 * a pointer to the string that represents the error. */
typedef const char *result_t;

/* Functions that return a `result_t` should return this if they ran successfully. */
#define SUCCESS 0

/* Generic assembler errors */

extern result_t ERR_PROGRAM_TOO_LARGE;
extern result_t ERR_PREPROCESS_FAILED;
extern result_t ERR_CODEGEN_FAILED;
extern result_t ERR_UNDEFINED_LABELS;
extern result_t ERR_INPUT_FILE_FAIL;
extern result_t ERR_OUTPUT_FILE_FAIL;
extern result_t ERR_OUT_OF_MEMORY;

/* Preprocessor errors */

extern result_t ERR_INVALID_MACRO_NAME;
extern result_t ERR_EXTRANEOUS_TEXT_MACRO;
extern result_t ERR_EXTRANEOUS_TEXT_MCROEND;
extern result_t ERR_LINE_TOO_LONG;
extern result_t ERR_FSEEK_FAILED;

/* Instruction syntax errors */

extern result_t ERR_UNKNOWN_INSTRUCTION;
extern result_t ERR_UNKNOWN_DIRECTIVE;
extern result_t ERR_NOT_ENOUGH_ARGS;
extern result_t ERR_TOO_MANY_ARGS;
extern result_t ERR_SRC_IMMEDIATE;
extern result_t ERR_SRC_REGISTER;
extern result_t ERR_DST_IMMEDIATE;

/* Argument syntax errors */

extern result_t ERR_NO_ARG;
extern result_t ERR_EXTRANEOUS_TEXT_INSTRUCTION;
extern result_t ERR_EXTRANEOUS_TEXT_DIRECTIVE;
extern result_t ERR_INVALID_ARGUMENT;
extern result_t ERR_INVALID_DIRECTIVE_SYNTAX;
extern result_t ERR_DIRECTIVE_EXPECTED_LABEL;

/* Number syntax errors */

extern result_t ERR_NUMBER_NOT_VALID;
extern result_t ERR_NUMBER_AFTER_HASH;
extern result_t ERR_IMMEDIATE_TOO_SMALL;
extern result_t ERR_IMMEDIATE_TOO_LARGE;
extern result_t ERR_WORD_TOO_SMALL;
extern result_t ERR_WORD_TOO_LARGE;

/* Label syntax errors */

extern result_t ERR_INVALID_LABEL;
extern result_t ERR_LABEL_UNDERSCORES;
extern result_t ERR_LABEL_TOO_LONG;
extern result_t ERR_LABEL_ALREADY_DEFINED;
extern result_t ERR_LABEL_NAME_IS_MACRO;
extern result_t ERR_LABEL_NOT_DEFINED;
extern result_t ERR_LABEL_ALREADY_ENTRY;
extern result_t ERR_LABEL_MISSING_SPACE;

/* Matrix syntax errors */

extern result_t ERR_MATRIX_OVERFLOW;
extern result_t ERR_MATRIX_START_BRACKET_ROW;
extern result_t ERR_MATRIX_END_BRACKET_ROW;
extern result_t ERR_MATRIX_START_BRACKET_COL;
extern result_t ERR_MATRIX_END_BRACKET_COL;
extern result_t ERR_MATRIX_NEGATIVE_STORAGE;
extern result_t ERR_WRONG_MATRIX_ACCESS;

/* String syntax errors */

extern result_t ERR_STRING_MISSING_QUOTE_START;
extern result_t ERR_STRING_MISSING_QUOTE_END;

/**
 * This macro can only be called in a function that returns a `result_t`.
 * Checks that the condition is true. If not, causes the running function to return the given error message as a
 * `result_t`.
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
 * Prints an error message, with a file path and line number.
 *
 * @param file_path The path of the file where the error was found.
 * @param line_number The number of the line where the error was found.
 * @param message A message describing the error.
 */
void print_error(char *file_path, int line_number, const char *message);

#endif
