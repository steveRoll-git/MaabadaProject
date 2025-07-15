#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "errors.h"
#include "table.h"

/* Preprocesses the file at the input path, and outputs the result to the output file. */
/* Returns whether it was successful (no errors encountered). */
result_t preprocess(char *input_file_path, char *output_file_path, table_t *macro_table);

#endif
