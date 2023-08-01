#include "instruction.h"


void fetch(struct typeSOC* soc, struct typeInstruction* ins) {
  ins->fetch = (soc->memory[soc->pc] <<8) | soc->memory[soc->pc +1];
  predecode(ins);
}

void predecode(struct typeInstruction* ins) {
  ins->opcode  = ins->fetch >> 12; 
  ins->nibble2 = (ins->fetch >> 8) & 0xF; 
  ins->nibble3 = (ins->fetch >> 4) & 0xF; 
  ins->nibble4 = ins->fetch & 0xF; 
  ins->address = ins->fetch & 0xFFF; 
  ins->byte    = ins->fetch & 0xFF; 

  ins->X       = ins->nibble2;
  ins->key     = ins->nibble2;
  ins->Y       = ins->nibble3;
  ins->opcode2 = ins->nibble4;
  ins->N       = ins->nibble4;
  ins->NN      = ins->byte;
  ins->NNN     = ins->address;



}
