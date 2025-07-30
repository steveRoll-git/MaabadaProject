#ifndef OUTPUT_H
#define OUTPUT_H

#include "context.h"

/**
 * Outputs the object file (".ob"), containing the assembler's code and data image in base 4 format.
 * May fail if:
 * - The output file failed to open.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @param out_path The path to the .ob file to output into.
 * @return The operation's result.
 */
result_t output_object(context_t *context, char *out_path);

/**
 * Outputs the entries (".ent") and externals (".ext") files for the assembler.
 * May fail if:
 * - The output files failed to open.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @param entries_path The path to the .ent file to output entries into.
 * @param externals_path The path to the .ext file to output externals into.
 * @return The operation's result.
 */
result_t output_entries_externals(context_t *context, char *entries_path, char *externals_path);

#endif
