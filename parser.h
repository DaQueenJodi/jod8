#pragma once
#include <stdint.h>
#include <stdbool.h>
typedef uint16_t Word;

typedef enum {
	CLS,
	RET,
	JP,
	CALL,
	SE,
	SNE
} Opcode;

typedef struct {
	Opcode opcode;
	uint8_t args[3];
} Instruction;



bool test_cmp(Word w, uint16_t cmp);
char *opcode_str(Opcode opcode);
Instruction parse_instruction(Word w);
