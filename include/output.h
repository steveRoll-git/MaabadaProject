#ifndef OUTPUT_H
#define OUTPUT_H

#include "assembler.h"

/* Outputs the object file (".ob"), containing the assembler's code and data image in base 4 format. */
void output_object(assembler_t *assembler, char *out_path);

#endif
