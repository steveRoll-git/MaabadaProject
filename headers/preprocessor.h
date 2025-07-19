#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "errors.h"
#include "table.h"

/* Preprocesses the file at the input path, and outputs the result to the output file. */
/* May fail if: */
/* - The input or output files failed to open. */
/* - Lines that are longer than 80 characters were found. */
/* - Syntax errors were encountered in preprocessor-specific lines, such as `mcro` and `mcroend`. */
/* - Memory allocations did not succeed. */
result_t preprocess(char *input_file_path, char *output_file_path, table_t *macro_table);

#endif
