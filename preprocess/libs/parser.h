#ifndef PREPROCESS_PARSER_H
#define PREPROCESS_PARSER_H

/* The various kinds of lines that the preprocessor's `parse_line` can give. */
typedef enum {
    LINE_NORMAL, /* A line with no special meaning to the preprocessor. */
    LINE_MCRO, /* A line that starts a macro definition with `mcro`. */
    LINE_MCROEND, /* A line that ends a macro definition with `mcroend`. */
    LINE_MACROCALL, /* A line that calls a macro by its name. */
    LINE_ERROR /* Indicates that an error occurred while reading this line. */
} parse_line_status_t;

/* The maximum size of a single line in bytes (80 characters and a null terminator.) */
#define MAX_LINE 81

/* Parses the given line, and returns what kind of line it is. If the line is a macro definition or call, stores the macro's name in `macro_name`. */
parse_line_status_t parse_line(char line[MAX_LINE], char *macro_name, int print_errors);

/* Returns whether the given string is the name of an assembly instruction. */
int is_keyword(char *token);

#endif
