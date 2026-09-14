CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c99 -g

.PHONY: all example output check clean

all: example output

example: example.c solog.h
	$(CC) $(CFLAGS) example.c -o $@

output: output.c solog.h
	$(CC) $(CFLAGS) output.c -o $@

check:
	$(MAKE) -C test check

clean:
	rm -f example output example.exe output.exe
	$(MAKE) -C test clean
