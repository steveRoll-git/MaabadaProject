#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "data.h"

/* Preprocesses the file at the input path, and outputs the result to the output file. */
/* Returns whether it was successful (no errors encountered). */
bool_t preprocess(char *input_file_path, char *output_file_path);

#endif
