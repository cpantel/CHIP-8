#ifndef __instruction__
#define __instruction__

#include <stdint.h>
#include "soc.h"

struct typeInstruction {
    uint16_t fetch;	

    uint8_t  opcode;

    uint8_t  nibble2;
    uint8_t  X;
    uint8_t  key;

    uint8_t  nibble3;
    uint8_t  Y;

    uint8_t  nibble4;
    uint8_t  opcode2;
    uint8_t  N;

    uint16_t address;
    uint8_t  byte;
    uint8_t  NN;
    uint16_t  NNN;
};

void fetch(struct typeSOC* soc, struct typeInstruction* ins);
void predecode(struct typeInstruction* ins);


#endif
