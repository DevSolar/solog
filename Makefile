CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c99 -g

.PHONY: all example check clean

all: example

example: example.c solog.h
	$(CC) $(CFLAGS) example.c -o $@

check:
	$(MAKE) -C test check

clean:
	rm -f example
	$(MAKE) -C test clean
