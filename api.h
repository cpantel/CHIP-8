#ifndef __API__
#define __API__

#include "soc.h"
#include "instruction.h"

enum Opcode {
  OPCODE_CLS,    // 00E0
  OPCODE_RET,    // 00EE
  OPCODE_JMP,    // 1NNN
  OPCODE_CALL,   // 2NNN
  OPCODE_SNE,    // 3XNN
  OPCODE_SEQ,    // 4XNN
  OPCODE_SCNE,   // 5XNN
  OPCODE_LD,     // 6XNN
  OPCODE_ADD,    // 7XNN
  OPCODE_CP,     // 8XY0
  OPCODE_OR,     // 8XY1
  OPCODE_AND,    // 8XY2
  OPCODE_XOR,    // 8XY3
  OPCODE_ADDC,   // 8XY4
  OPCODE_SUB,    // 8XY5
  OPCODE_SHR,    // 8XY6
  OPCODE_SUBI,   // 8XY7
  OPCODE_SHL,    // 8XYe
  OPCODE_SCEQ,   // 9XY0
  OPCODE_LDI,    // ANNN
  OPCODE_JMPO,   // BNNN
  OPCODE_RND,    // CXNN
  OPCODE_SPRITE, // DXYN
  OPCODE_SKNP,   // EX9E
  OPCODE_SKP,    // EXA1
  OPCODE_GETT,   // FX07
  OPCODE_WKP,    // FX0A
  OPCODE_SETT,   // FX15
  OPCODE_SETB,   // FX18
  OPCODE_ADDI,   // FX1E
  OPCODE_SETI,   // FX29
  OPCODE_BCD,    // FX33
  OPCODE_SAVE,   // FX54
  OPCODE_LOAD    // FX65
};

void api(struct typeSOC* soc, struct typeInstruction* ins, enum Opcode opcode);

#endif
