BINARY=main
CODEDIRS=. lib
INCDIRS=. ./include/

CC=gcc

DEPFLAGS=-MP -MD

CFLAGS=-Wall -Wextra -pedantic -g $(foreach D, $(INCDIRS), -I $(D)) $(OPT) $(DEPFLAGS)

CFILES= $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))

OBJECTS=$(patsubst %.c, %.o, $(CFILES))
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^