//
// Created by Yontank on 03/07/2025.
//

#ifndef ERR_H
#define ERR_H

#define ERR_EXTRANOUS_INFORMATION_AFTER_ARGUEMENTS "Extranous Data"
#define ERR_INVALID_ARGUEMENT "Invalid Argument"

/* Instructions */
#define ERR_FIRST_ARG_INVALID "Invalid First Argument"
#define ERR_SECOND_ARG_INVALID "Invalid Second Argument"
#define ERR_WHERE_IS_MY_COMMA "Missing Comma between two commands"
#define ERR_UNKNOWN_INSTRUCTION "Unknown Assembly Command"
#define ERR_INVALID_INSTRUCTION "Invalid Instruction"
#define ERR_INVALID_COMMAND "Invalid Command"
#define ERR_UNKNOWN_DIRECTIVE "Unknown directive"


/* Numbers */
#define ERR_NUMBER_NOT_VALID "Invalid Number"
#define ERR_NUMBER_FIRST_CHAR_WRONG "First Character of number isn't [+\\-]|[1-9]"

/*Labels*/
#define ERR_INVALID_LABEL "Invalid Label"
#define ERR_LABEL_UNDERSCORES "Label name cannot contain underscores"
#define ERR_LABEL_TOO_LONG "Label name is longer than 30 characters"

/* Matrix*/
#define ERR_MATRIX_OVERFLOW "Matrix has too many values for its buffer size"
#define ERR_MATRIX_START_BRACKET_ROW "Missing '[' for row index."
#define ERR_MATRIX_END_BRACKET_ROW "Missing ']' for row index."
#define ERR_MATRIX_START_BRACKET_COL "Missing '[' for column index."
#define ERR_MATRIX_END_BRACKET_COL "Missing ']' for column index."
#define ERR_MATRIX_NEGATIVE_STORAGE "Matrix Rows And cols must be larger than 0"
#define ERR_WRONG_MATRIX_ACCESS "Matrix rows and columns can only be accessed with registers"


/*String*/
#define ERR_STRING_MISSING_QUOTE "Missing quote sign"

/* Arguments */
#define ERR_NO_ARG "Missing Argument"

/* Checks that the condition is true. If it's not, causes the running function to return the given error message as a
 * `result_t`. */
#define ASSERT(a, s)                                                                                                   \
  if (!(a)) {                                                                                                          \
    return (result_t) s;                                                                                               \
  }

/* The given expression should be one of `result_t` type. If it does not return `SUCCESS`, causes the running function
 * to return the given error string. */
#define TRY_M(a, s)                                                                                                    \
  if ((a) != SUCCESS) {                                                                                                \
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

/* Any function that can succeed or fail returns this type. If the function succeeds, it returns 0, otherwise it returns
 * a pointer to the string that represents the error. */
typedef char *result_t;
#define SUCCESS 0

#endif // ERR_H
