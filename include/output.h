#ifndef OUTPUT_H
#define OUTPUT_H

#include "assembler.h"

/* Outputs the object file (".ob"), containing the assembler's code and data image in base 4 format. */
/* May fail if: */
/* - The output files failed to open. */
/* - Memory allocations did not succeed. */
result_t output_object(assembler_t *assembler, char *out_path);

/* Outputs the entries (".ent") and externals (".ext") files for the assembler. */
/* May fail if: */
/* - The output files failed to open. */
/* - Memory allocations did not succeed. */
result_t output_entries_externals(assembler_t *assembler, char *entries_path, char *externals_path);

#endif
