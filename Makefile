SFLAGS=-fsanitize=address,leak,undefined
SDL=$(shell pkgconf --libs --cflags sdl2)
CFLAGS=-Wextra -Wall -Wpedantic $(SDL)  $(SFLAGS)

SRCS=$(shell find -name "*.c")
BIN=bin/chip8

CC=clang

all: bin
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)

bin:
	mkdir bin

run: $(BIN)
	./$(BIN)
