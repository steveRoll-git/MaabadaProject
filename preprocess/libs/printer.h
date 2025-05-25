#include <stdio.h>  
/*Prints a line into our output file*/
void print_line(FILE* out, char* token);

/*Given an input file, prints all important information untill it gets into a **mcroend** token.*/
void print_macro(FILE* out, FILE* in);
