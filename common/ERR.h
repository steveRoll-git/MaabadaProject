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


/* Numbers */
#define ERR_NUMBER_NOT_VALID "Invalid Number"

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


/*String*/
#define ERR_STRING_MISSING_QUOTE "Missing quote sign"


#define ASSERTM(a, s)                                                                                                  \
  if (!(a)) {                                                                                                          \
    fprintf(stderr, "Error: %s ", s);                                                                                  \
    return 0;                                                                                                          \
  }

#define ASSERT(a)                                                                                                      \
  if (!(a)) {                                                                                                          \
    return 0;                                                                                                          \
  }


#endif // ERR_H
