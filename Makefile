SFLAGS=-fsanitize=address,leak,undefined
CFLAGS=-Wextra -Wall -Wpedantic $(SFLAGS)

SRCS=$(shell find -name "*.c")
BIN=bin/chip8

CC=clang

all: bin
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)

bin:
	mkdir bin

run: $(BIN)
	./$(BIN)
