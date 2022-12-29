#pragma once
#include <stdbool.h>
#include <stdint.h>
typedef uint16_t Word;

typedef enum {
  CLS,
  RET,
  JP,
  CALL,
  SEByte,
  SNEByte,
  SE,
  SNE,
	LD,
	LDByte,
	LDIndex,
	LDToDelay,
	LDFromDelay,
	LDKey,
	LDSound,
	LDSprite,
	LDBCD,
	LDToArray,
	LDFromArray,
	ADD,
	ADDByte,
	ADDIndex,
	AND,
	OR,
	XOR,
	SUB,
	SHR,
	SUBN,
	SHL,
	JPOffset,
	DRW,
	SKP,
	SKNP,
	RND,
	NOP,
} Opcode;

typedef struct {
  Opcode opcode;
  uint8_t args[3];
} Instruction;

bool test_cmp(Word w, uint16_t cmp);
char *opcode_str(Opcode opcode);
Instruction parse_instruction(Word w);
