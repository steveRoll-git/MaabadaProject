#ifndef CODEGEN_H
#define CODEGEN_H

#include "context.h"

/**
 * Goes over the input file, and generates binary code for all the instructions and directives.
 * Binary code for label operands isn't generated yet.
 * The values of labels that were found, and references to them, are stored in the label table.
 * May fail if:
 * - The input or output files failed to open.
 * - Syntax errors or semantical errors were detected in the source file.
 * - Memory allocations did not succeed.
 *
 * @param context The assembler context to operate on.
 * @return The operation's result.
 */
result_t codegen(context_t *context);

#endif
