CC=gcc
INCDIRS=. ./include/
CODEDIR=. ./libs/
CFLAGS=-Wall -Wextra -pedantic -g -ansi $(INCDIRS) $(OPT)

 

preproc:  macroprocessor.c libs/parser.c ../common/linked_list.c
	gcc macroprocessor.c libs/parser.c ../common/linked_list.c  -Wall -pedantic -o preproc -g 

