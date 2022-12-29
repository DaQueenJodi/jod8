#include "frontends/frontends.h"
#include "parser.h"
#include <assert.h>
#include <endian.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG
#define MEM_START 0x200

#define LOG(...)                                                               \
  fprintf(stderr, "%s:%d: ", __FILE__, __LINE__);                              \
  fprintf(stderr, __VA_ARGS__);                                                \
  puts("\n")

enum STATUS_FLAGS {
  CARRY,
  NOT_BORROW,
};

uint16_t STACK[16];
uint8_t SP;
uint16_t PC;
uint8_t REGISTERS[16];
uint16_t IR;
uint8_t DELAY_TIMER;
uint8_t FRAMEBUFFER[32 * 64];

uint8_t MEMORY[4095]; // 4kb

void setup(void) {
  memset(STACK, 0, sizeof(STACK));
  memset(FRAMEBUFFER, 0, sizeof(FRAMEBUFFER));
  memset(REGISTERS, 0, sizeof(REGISTERS));
  memset(MEMORY, 0, sizeof(MEMORY));
  SP = 0;
  PC = MEM_START;
  IR = 0;
}

#define REG(num) REGISTERS[num]
#define VF REG(0xF)

#define ARGS_TO_ADDR(args) ((args[0] * 256) + (args[1] * 16) + (args[2]))

void do_instruction(Instruction inst) {
  uint8_t *args = inst.args;
  switch (inst.opcode) {
  case NOP:
    break;
  case CLS: {
    memset(FRAMEBUFFER, 0, sizeof(FRAMEBUFFER));
    LOG("cleared the screen");
    break;
  }
  case RET: {
    uint16_t old_pc = PC;
    PC = STACK[SP--];
    LOG("returned from %hu to %hu", old_pc, PC);

    break;
  }
  case JP: {
    LOG("ARGS: %hhu, %hhu, %hhu", args[0], args[1], args[2]);
    uint16_t old_pc = PC;
    PC = ARGS_TO_ADDR(args);
    LOG("jumped from %hx to %hx", old_pc, PC);
    break;
  }
  case CALL: {
    STACK[++SP] = PC;
    PC = ARGS_TO_ADDR(args);
    LOG("calling %hx from %hx", PC, STACK[SP]);
  }
  case SEByte: {
    if (args[0] == ((args[1] * 16) + (args[2] * 1))) {
      PC += 2;
    }
    break;
  }
  case SNEByte: {
    if (args[0] != ((args[1] * 16) + (args[2] * 1))) {
      PC += 2;
    }
    break;
  }
  case SE: {
    if (REG(args[0]) == REG(args[1])) {
      PC += 2;
    }
    break;
  }
  case SNE: {
    if (REG(args[0]) != REG(args[1])) {
      PC += 2;
    }
    break;
  }
  case LDByte: {
    REG(args[0]) = (args[1] * 16) + (args[2] * 1);
    break;
  }
  case ADDByte: {
    REG(args[0]) += (args[1] * 16) + (args[2] * 1);
    break;
  }
  case LD: {
    REG(args[0]) = REG(args[1]);
    break;
  }
  case OR: {
    REG(args[0]) |= REG(args[1]);
    break;
  }
  case AND: {
    REG(args[0]) &= REG(args[1]);
    break;
  }
  case XOR: {
    REG(args[0]) ^= REG(args[1]);
    break;
  }
  case ADD: {
    uint16_t test = REG(args[0]) + REG(args[1]);
    if (test > 255) {
      VF = 1;
      REG(args[0]) = (test & 0xF0); // only keep lowest 8 bits
    } else {
      VF = 0;
      REG(args[0]) = test;
    }
    break;
  }
  case SUB: {
    short test = REG(args[0]) + REG(args[1]);
    if (test < 0) {
      REG(args[0]) = (test & 0xF0);
      VF = 1;
    } else {
      REG(args[0]) = test;
      VF = 0;
    }
    break;
  }
  case SHR: {
    REG(args[0]) <<= REG(args[1]);
    VF = (REG(args[0]) & 1);
    break;
  }
  case JPOffset: {
    uint16_t old_pc = PC;
    PC = REG(0) + ARGS_TO_ADDR(args);
    LOG("jumped from %hx to %hx", old_pc, PC);
    break;
  }
  case DRW: {
    uint8_t startx = REG(args[0]);
    uint8_t starty = REG(args[1]);
    uint8_t n = args[2];

    LOG("drawing %hhu bytes at %hhu, %hhu", n, startx, starty);

    for (size_t i = 0; i < n; i++) {
      uint8_t *byte = &FRAMEBUFFER[(starty * 32) + startx];
      if ((*byte & MEMORY[IR + i]) == 0) { // check if any bits will be zeroed
        VF = 0;
      } else {
        VF = 1;
      }
      *byte ^= MEMORY[IR + i];
    }
    break;
  }
  case LDIndex: {
    uint16_t addr = ARGS_TO_ADDR(args);
    LOG("set IR to %hu, from %hu", addr, IR);
    IR = addr;
    break;
  }
  case LDToArray: {
    uint8_t n = args[0];
    for (size_t i = 0; i < n; i++) {
      MEMORY[IR + i] = REG(i);
    }
    break;
  }
  case LDFromArray: {
    uint8_t n = args[0];
    for (size_t i = 0; i < n; i++) {
      REG(i) = MEMORY[IR + i];
    }
    break;
  }
  case LDBCD: {
    uint8_t n = args[0];
    size_t counter = 0;
    do {
      n = floor(n /= 10);
      MEMORY[IR + counter++] = n;
    } while (n != 0);
    break;
  }
  default: {
    LOG("unimplemented: %d", inst.opcode);
    exit(1);
  }
  }
}

void load_file(char *path) {
  FILE *f = fopen(path, "rb");
  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  rewind(f);
  uint8_t *buff = &MEMORY[0x200];
  if (fread(buff, len, 1, f) == 0) {
    perror("failed to read file");
    exit(1);
  }
  fclose(f);
}

int main(void) {
	setup();
	gui_frontend_init();
	memset(FRAMEBUFFER, 0b1010101010, 1);
	gui_frontend_draw_framebuffer(FRAMEBUFFER);
	while (true) {
		
	}
}

int main2(void) {
  setup();
  gui_frontend_init();
  load_file("test");

  while (PC <= sizeof(MEMORY) * 8) {
    printf("PC: 0x%hx\n", PC);
    Word word = *(uint16_t *)&MEMORY[PC];
    PC += 2;
    word = htobe16(word);
    printf("word: %x \n", word);
    Instruction inst = parse_instruction(word);
    LOG("word: %hx, instruction: %s", word, opcode_str(inst.opcode));
    do_instruction(inst);
		gui_frontend_draw_framebuffer(FRAMEBUFFER);
		//	sleep(1);
  }
  gui_frontend_cleanup();
  return 0;
}
