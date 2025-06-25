
#include "../common/linked_list.h"
#include "data.h"
#include <stdio.h>

const struct instruction_t arr[];

/*Given an instruction line inside the assembly, convert the instruction type
 * into its coresponding decimal value.
 *
 * returns -1 if the instruction isn't known.*/
int keyword_to_value(char *token);

/**
 * Given a token inside an assembly command, check if it's a label.
 * returns -1 if the string given is NULL.
 * otherwise :
 * returns 1 if its a label, 0 otherwise.
 *
 * NOTE: remember this function only checks if its a label, to check if the
 * label string is valid, please clal is_label_valid.
 */
int is_label(char *token);

/**
 * Checks if label is valid, by checking the following things:
 * 1) is the first character alphanumeric? [a-zA-Z].
 * 2) is the label name already used as a macro name?
 * 3) is the label name already used as a label?
 *
 * if any of those checks are true, the label isn't valid and returns 0.
 * if it's valid it returns 1.
 */
int is_label_valid(char *label, linked_list_t *macro_table,
                   linked_list_t *label_table, linked_list_t *data_table);

/*
 * specific parser we've created to read lines,
 * the command reads lines from a given file and stops  the moment we the end of
 * a line (Meaning '\n', or EOF). if the line received is longer than 80 bytes,
 * the buffer won't receive any more bytes and return 0. otherwise it will
 * return 1.
 * */

sentence_t read_line(FILE *file, char line[MAX_LINE]);

/*Checks if given an assembly command is a known instruction type.
 * returns 1 if it is an known instruction string
 * 0 otherwise*/
int is_assembly_command(char *token);
