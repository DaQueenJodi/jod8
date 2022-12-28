#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

char *opcode_str(Opcode opcode) { assert(0 && "Not Implemented!"); }


static void fill_args(int stop, uint8_t *args, Word w) {
	for (int i = 0; i < stop; i++) {
		Word mask;
		switch (i) {
			mask = 0x0F00;
		}
	}
}

Instruction parse_instruction(Word w) {
  Instruction inst;

  if ((w & 0xF0) == 0x00) {
    switch (w & 0x0F) {
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
}
