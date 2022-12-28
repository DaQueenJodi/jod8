#include <stdio.h>
#include <stdint.h>
//#include "parser.h"

#define MEM_START 0x200

uint8_t STACK[64];
uint8_t SP;
uint16_t PC;
uint8_t REGISTERS[16];
uint16_t IR;
uint8_t DELAY_TIMER;
uint8_t FRAMEBUFFER[256];



int main(void) {
	printf("%x \n", 0xEFAE & 0x00F0);
	return 0;
}
