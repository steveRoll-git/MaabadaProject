#ifndef CODEGEN_H
#define CODEGEN_H

#include "assembler.h"
#include "data.h"

/* Goes over the input file, and generates binary code for all the instructions and directives.
 * Binary code for label operands isn't generated yet.
 * The values of labels that were found, and references to them, are stored in the label table. */
bool_t codegen(assembler_t *assembler);

#endif
