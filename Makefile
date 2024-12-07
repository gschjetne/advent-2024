CFLAGS = -g -Wall

LIB_SOURCES = $(wildcard lib/*.c)
LIB_OBJECTS = $(addprefix obj/,$(notdir $(LIB_SOURCES:.c=.o)))

SOLUTION_SOURCES = $(wildcard src/*.c)
SOLUTIONS = $(addprefix bin/,$(notdir $(SOLUTION_SOURCES:.c=)))

obj/%.o: lib/%.c lib/%.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<

bin/%: src/%.c $(LIB_OBJECTS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $< $(LIB_OBJECTS)

all: $(SOLUTIONS)

clean:
	rm -rf obj bin

.PHONY: clean dirs
.PRECIOUS: obj/%.o
