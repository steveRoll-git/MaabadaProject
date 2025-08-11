# Binary name
BINARY = assembler

# Compiler
CC = gcc

# Compilation flags
CFLAGS = -Wall -Wextra -ansi -pedantic

assembler: assembler.o codegen.o context.o errors.o list.o output.o parser.o preprocessor.o table.o utils.o
	$(CC) $(CFLAGS) $^ -o assembler

assembler.o: source/assembler.c headers/codegen.h headers/context.h headers/output.h headers/preprocessor.h headers/utils.h
	$(CC) $(CFLAGS) -c source/assembler.c -o $@

codegen.o: source/codegen.c headers/codegen.h headers/context.h headers/data.h headers/parser.h
	$(CC) $(CFLAGS) -c source/codegen.c -o $@

context.o: source/context.c headers/context.h headers/data.h headers/errors.h headers/list.h headers/table.h headers/utils.h
	$(CC) $(CFLAGS) -c source/context.c -o $@

errors.o: source/errors.c headers/errors.h
	$(CC) $(CFLAGS) -c source/errors.c -o $@

list.o: source/list.c headers/list.h headers/errors.h headers/utils.h
	$(CC) $(CFLAGS) -c source/list.c -o $@

output.o: source/output.c headers/output.h headers/context.h
	$(CC) $(CFLAGS) -c source/output.c -o $@

parser.o: source/parser.c headers/parser.h headers/data.h headers/errors.h headers/utils.h
	$(CC) $(CFLAGS) -c source/parser.c -o $@

preprocessor.o: source/preprocessor.c headers/preprocessor.h headers/errors.h headers/table.h headers/parser.h
	$(CC) $(CFLAGS) -c source/preprocessor.c -o $@

table.o: source/table.c headers/table.h headers/errors.h headers/list.h headers/utils.h
	$(CC) $(CFLAGS) -c source/table.c -o $@

utils.o: source/utils.c headers/utils.h headers/errors.h
	$(CC) $(CFLAGS) -c source/utils.c -o $@

clean:
	rm -f *.o