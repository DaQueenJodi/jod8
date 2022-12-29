#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

char *opcode_str(Opcode opcode) {
  switch (opcode) {
  case CLS:
    return "CLS";
  case RET:
    return "RET";
  case JP:
    return "JP";
  case CALL:
    return "SE";
  case SNE:
    return "SNE";
  }
}

#define FILL_ARGS(num)                                                         \
  {                                                                            \
    for (int i = 0; i < num; i++) {                                            \
      inst.args[i] = (w & (0x0F00) >> i * 4);                                  \
    }                                                                          \
  }

Instruction parse_instruction(Word w) {
  Instruction inst;
	inst.args = {0};

  if ((w & 0xFF00) == 0x00) {
    switch (w & 0x00FF) {
    case 0xE0: {
      inst.opcode = CLS;
      break;
    }
    case 0xEE: {
      inst.opcode = RET;
      break;
    }
    default: {
      assert(0 && "unreachable");
    }
    }
  } else {

    switch (w & 0xF000) {
    case 1: {
      inst.opcode = JP;
      FILL_ARGS(3);
      break;
    }
    case 2: {
      inst.opcode = CALL;
      FILL_ARGS(3);
      break;
    }
    case 3: {
      inst.opcode = SE;
      FILL_ARGS(3);
      break;
    }
    case 4: {
      inst.opcode = SNE;
      FILL_ARGS(3);
      break;
    }
    default: {
      assert(0 && "not implemented");
    }
    }
  }
  return inst;
}
