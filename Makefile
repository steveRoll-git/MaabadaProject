# Binary Name
BINARY=assembler
# C Code
CODEDIRS=. source
# Headers
INCDIRS=. include
# Compiler
CC=gcc
# Include Headers while compiling (whenever we change code)
DEPFLAGS=-MP -MD
# Flags for files
CFLAGS=-Wall -Wextra -pedantic -g $(foreach D, $(INCDIRS), -I $(D)) $(OPT) $(DEPFLAGS)
# All the C Files
CFILES= $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))
# Converts *.c to *.o, name, for later use so we can have easier time remembeing object names.
OBJECTS=$(patsubst %.c, %.o, $(CFILES))
# Dependancy files
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

all: $(BINARY)
	echo $(CFLAGS)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(BINARY) $(OBJECTS) $(DEPFILES)
