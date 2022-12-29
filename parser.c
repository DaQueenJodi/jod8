#include "parser.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char *opcode_str(Opcode opcode) {
  // TODO: decide if this should be SNEB or SNE.b or something
  switch (opcode) {
    // clang-format off
  case CLS			: return "CLS"  ;
  case RET					: return "RET"  ;
  case JP						: return "JP"		;
  case CALL					: return "CALL"	;
  case SE						: return "SE"		;
  case SNEByte			: return "SNE"	;
  case SNE					: return "SNE"	;
  case SEByte				: return "SE"		;
  case LD						: return "LD"		;
	case LDByte				: return "LD"		;
	case LDIndex			: return "LD"		;
	case LDToDelay		: return "LD"		;
	case LDFromDelay	: return "LD"		;
	case LDKey				: return "LD"		;
	case LDSound			: return "LD"		;
	case LDSprite			: return "LD"		;
	case LDBCD				: return "LD"		;
	case LDToArray		: return "LD"		;
	case LDFromArray	: return "LD"		;
	case ADD					: return "ADD"  ;
	case ADDByte					: return "ADD"  ;
	case ADDIndex					: return "ADDI" ;
	case AND					: return "AND"  ;
	case OR						: return "OR"   ;
	case XOR					: return "XOR"  ;
	case SUB					: return "SUB"	;
	case SUBN					: return "SUBN" ;
	case SHR					: return "SHR"  ;
  case SHL					: return "SHL"  ;
	case JPOffset			: return "JP"   ;
	case DRW					: return "DRW"  ;
	case SKP					: return "SKP"  ;
	case SKNP					: return "SKNP" ;
	case RND					: return "RND"  ;
  case NOP					: return "NOP"  ;


    // clang-format on
  }
}

#define FILL_ARGS(num)                                                         \
  {                                                                            \
    for (int i = 0; i < num; i++) {                                        \
      inst.args[i] = (w & (0x0F00 >> i * 4)) >> (12 - (4 * (i + 1)));          \
    }                                                                          \
  }

Instruction parse_instruction(Word w) {
  Instruction inst;
  memset(inst.args, 0, sizeof(inst.args));

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
      inst.opcode = NOP;
      return inst;
    }
    }
  } else {

    switch (w & 0xF000) {
    case 0x1000: {
      inst.opcode = JP;
      FILL_ARGS(3);
      break;
    }
    case 0x2000: {
      inst.opcode = CALL;
      FILL_ARGS(3);
      break;
    }
    case 0x3000: {
      inst.opcode = SEByte;
      FILL_ARGS(3);
      break;
    }
    case 0x4000: {
      inst.opcode = SNEByte;
      FILL_ARGS(3);
      break;
    }
    case 0x5000: {
      inst.opcode = SE;
      FILL_ARGS(2);
      break;
    }
    case 0x6000: {
      inst.opcode = LDByte;
      FILL_ARGS(3);
      break;
    }
    case 0x7000: {
      inst.opcode = ADDByte;
      FILL_ARGS(3);
      break;
    }
    case 0x8000: {
      FILL_ARGS(2); // its always going to be 2 args
      switch (w & 0x000F) {
      case 0x0000: {
        inst.opcode = LDByte;
        break;
      }
      case 0x0001: {
        inst.opcode = OR;
        break;
      }
      case 0x0002: {
        inst.opcode = AND;
        break;
      }
      case 0x0003: {
        inst.opcode = XOR;
        break;
      }
      case 0x0004: {
        inst.opcode = ADD;
        break;
      }
      case 0x0005: {
        inst.opcode = SUB;
        break;
      }
      case 0x0006: {
        inst.opcode = SHR;
        break;
      }
      case 0x0007: {
        inst.opcode = SUBN;
        break;
      }
      case 0x000E: {
        inst.opcode = SHL;
        break;
      }
      default:
        assert(0 && "unreachable");
      }
    }
    case 0x9000: {
      inst.opcode = SNE;
      FILL_ARGS(2);
      break;
    }
    case 0xA000: {
      inst.opcode = LDIndex;
      FILL_ARGS(3);
      break;
    }
    case 0xB000: {
      inst.opcode = JP;
      FILL_ARGS(3);
      break;
    }
    case 0xC000: {
      inst.opcode = RND;
      break;
    }
    case 0xD000: {
      inst.opcode = DRW;
      FILL_ARGS(3);
      break;
    }
    case 0xE000: {
      switch (w & 0x00FF) {
        FILL_ARGS(1);
      case 0x009E: {
        inst.opcode = SKP;
        break;
      }
      case 0x00A1: {
        inst.opcode = SKNP;
        break;
      }
      default:
        assert(0 && "unreachable");
      }
      break;
    }
    case 0xF000: {
      FILL_ARGS(1);
      switch (w & 0x00FF) {
      case 0x0007: {
        inst.opcode = LDFromDelay;
        break;
      }
      case 0x000A: {
        inst.opcode = LDKey;
        break;
      }
      case 0x0015: {
        inst.opcode = LDToDelay;
        break;
      }
      case 0x0018: {
        inst.opcode = LDSound;
        break;
      }
      case 0x001E: {
        inst.opcode = ADDIndex;
        break;
      }
      case 0x0029: {
        inst.opcode = LDSprite;
        break;
      }
      case 0x0033: {
        inst.opcode = LDBCD;
        break;
      }
      case 0x0055: {
        inst.opcode = LDToArray;
        break;
      }
      case 0x0065: {
        inst.opcode = LDFromArray;
        break;
      }
      default:
        assert(0 && "unreachable");
      }
      break;
    }
    }
  }
  return inst;
}
