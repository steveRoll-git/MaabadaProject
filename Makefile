# Binary Name
BINARY=main
# C Code
CODEDIRS=. datatypes/ external/ firstpass/ preprocess/
# Headers
INCDIRS=. datatypes/ external/ firstpass/ preprocess/
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

### Tests
TEST_BINARY=tests/tester
TEST_LIB=tests/lib
TEST_CDIRS=tests/ tests/init

TEST_FILES_C=$(foreach D, $(TEST_CDIRS), $(wildcard $(D)/*.c))
TEST_OBJECTS=$(patsubst %.c, %.o, $(TEST_FILES_C))
TEST_FLAGS=$(foreach D, $(TEST_LIB), -L $(D)) -lunity

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(BINARY) $(OBJECTS) $(DEPFILES) $(TEST_BINARY) $(TEST_OBJECTS)

test: $(TEST_BINARY)

$(TEST_BINARY): $(TEST_OBJECTS)
	$(CC) $(CFLAGS)  -o $@ $^ $(TEST_FLAGS)
	./$@



