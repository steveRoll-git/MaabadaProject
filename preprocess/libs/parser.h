
#define MAX_LINE 81
/** Recieves a token from the user, which then gets analyzed to check which type
 * of line it is. Right now, there are 4 type of line 1) WhiteSpace line being
 * only filled with empty spaces 2) Comments, starting with semi-colon (;) 3)
 * Informative, Creating Variables, letting the user know about stuff. 4)
 * Commands, the raw ASM commands written.
 *
 */
void line_type(char *line);

/* FIXME: add global header for MAX_LINE length.*/

/** Receievs a Line of MAX_LENGTH size, and parses the information to check if
 * it's correct.*/
void parse_line(char line[MAX_LINE]);

int itk (char* token);
