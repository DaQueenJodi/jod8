#include "parser.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#define MEM_START 0x200

#ifdef DEBUG
#define LOG(...)                                                               \
  fprintf(stderr, __VA_ARGS__);                                                 \
  puts("\n")
#else
#define LOG(...)
#endif

uint8_t STACK[64];
uint8_t SP;
uint16_t PC;
uint8_t REGISTERS[16];
uint16_t IR;
uint8_t DELAY_TIMER;
bool FRAMEBUFFER[256];

void setup(void) {
  memset(STACK, 0, sizeof(STACK));
  memset(FRAMEBUFFER, 0, sizeof(FRAMEBUFFER));
  SP = 0;
  PC = MEM_START;
}

void do_instruction(Instruction inst) {
  switch (inst.opcode) {
  case CLS:
    memset(FRAMEBUFFER, 0, sizeof(FRAMEBUFFER));
    LOG("cleared the screen");
    break;
  }
case RET:
#ifdef DEBUG
  uint16_t old_pc = PC;
#endif
  PC = STACK[sp--];
#ifdef DEBUG
  LOG("returned from %hl to %hl", old_pc, PC);
#endif
  break;
case JP:
#ifdef DEBUG
  uint16_t old_pc = PC;
#endif
  PC = (inst.args[0] * 100) + (inst.args[1] * 10) + (inst.args[2]) * 1;
#ifdef DEBUG
  LOG("jumped from %hl to %hl", old_pc, PC);
#endif
}

int main(void) {
  setup();
  Word word = 0x00E0;
  Instruction inst = parse_instruction(word);
	do_instruction(inst);
  
  return 0;
}
