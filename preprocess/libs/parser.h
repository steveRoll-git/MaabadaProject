
#include "datatypes.h"
#include <stdio.h>
#define ASSERT(condition, message, is_critical)                                \
  if (!(condition)) {                                                          \
    printf("%s", message);                                                     \
    if (is_critical)                                                           \
      exit(1);                                                                 \
    return 1;                                                                  \
  }

typedef enum {IN_MACRO, NORMAL, MCROEND, MCALL} STATUS;
  
#define MAX_LINE 81

/** Recieves a token from the user, which then gets analyzed to check which type
 * of line it is. Right now, there are 4 type of line 1) WhiteSpace line being
 * only filled with empty spaces 2) Comments, starting with semi-colon (;) 3)
 * Informative, Creating Variables, letting the user know about stuff. 4)
 * Commands, the raw ASM commands written.
 *
 */
void line_type(char *line);

/** Receievs a Line of MAX_LENGTH size, and parses the information to check if
 * it's correct.*/
STATUS parse_line(char line[MAX_LINE], llm_t* table_name, FILE* in, FILE* out);

int itk(char *token);
