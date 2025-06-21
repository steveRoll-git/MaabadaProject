CC=gcc
INCDIRS=. ./include/
CODEDIR=. ./libs/
CFLAGS=-Wall -Wextra -pedantic -g -ansi $(INCDIRS) $(OPT)

 

preproc:  macroprocessor.c libs/parser.c libs/datatypes.c libs/printer.c
	gcc macroprocessor.c libs/parser.c libs/datatypes.c libs/printer.c -Wall -pedantic -o preproc -g

