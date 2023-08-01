#include "stdio.h"
#include "stdlib.h"

#include "api.h"
#include "soc.h"
#include "instruction.h"


void api (struct typeSOC* soc, struct typeInstruction* ins, enum Opcode opcode) {
  switch(opcode) {
    case OPCODE_CLS: // 0x00E0
      soc_clear_screen(soc,0);
    break;  

    case OPCODE_RET: // 0x00EE
      soc->pc = soc->stack[soc->stack_pointer-1];
      (soc->stack_pointer)--;
    break;

    case OPCODE_JMP: // 0x1NNN
      soc->pc = ins->address;
    break;	    

    case OPCODE_CALL: // 0x2NNN
      soc->stack[soc->stack_pointer++]=soc->pc;
      soc->pc = ins->address;
    break;

    case OPCODE_SEQ: // 0x3XNN
      if (soc->v[ins->X] == ins->NN) soc->pc+=2;
    break;

    case OPCODE_SNE: // 0x4XNN
      if (soc->v[ins->X] != ins->NN) soc->pc+=2;
    break;

    case OPCODE_SCEQ: // 0x5XY0
      if (soc->v[ins->X] == soc->v[ins->Y]) soc->pc+=2;
    break;

    case OPCODE_LD: // 0x6XNN
      soc->v[ins->X] = ins->NN;
    break;

    case OPCODE_ADD: // 0x7XNN
      soc->v[ins->X]+= ins->NN;
    break;

    case OPCODE_CP: // 0x8XY0
      soc->v[ins->X] = soc->v[ins->Y];
    break;

    case OPCODE_OR: // 0x8XY1
      soc->v[ins->X] |= soc->v[ins->Y];
    break;

    case OPCODE_AND: // 0x8XY2
      soc->v[ins->X] &= soc->v[ins->Y];
    break;

    case OPCODE_XOR: // 0x8XY3
      soc->v[ins->X] ^= soc->v[ins->Y];
    break;

    case OPCODE_ADDC: // 0x8XY4
     { 
          uint16_t result = soc->v[ins->X] + soc->v[ins->Y];
          soc->v[ins->X] = result;
          soc->v[0xF]= result >> 8;
     }
    break;

    case OPCODE_SUB: // 0x8XY5
      {
          uint16_t X = soc->v[ins->X];
          uint16_t Y = soc->v[ins->Y];
	  uint16_t result = X - Y;
	  soc->v[ins->X] = result;
	  soc->v[0xF] = ! ( (result >> 8 ) & 0x1 ) ;
      }
    break;

    case OPCODE_SHR: // 0x8XY6
      {
          uint8_t carry = soc->v[ins->Y] & 0x1;
          soc->v[ins->X] = soc->v[ins->Y];
          soc->v[ins->X] = soc->v[ins->Y] >> 1;
	  soc->v[0xF] = carry;
      }	  
    break;

    case OPCODE_SUBI: // 0x8XY7
      {
          uint16_t X = soc->v[ins->X];
          uint16_t Y = soc->v[ins->Y];
	  uint16_t result = Y - X;
	  soc->v[ins->X] = result;
	  soc->v[0xF] = ! (  (result >> 8 ) & 0x1 );
      }
    break;

    case OPCODE_SHL: // 0x8XYE
      {
        uint8_t carry = ( soc->v[ins->Y] ) >> 7;
	soc->v[ins->X] = soc->v[ins->Y];
        soc->v[ins->X] = soc->v[ins->X] << 1;
	soc->v[0xF] = carry;
      }
    break;

    case OPCODE_SCNE: // 0x9XY0
      if (soc->v[ins->X] != soc->v[ins->Y]) soc->pc+=2;
    break;

    case OPCODE_LDI: // 0xANNN
      soc->i = ins->address;
    break;

    case OPCODE_JMPO: // 0xBNNN
      soc->pc = soc->v[0] + ins->NNN;
      //       soc->pc = soc->v[ins->X] + ins->NNN;
    break;

    case OPCODE_RND:
      soc->v[ins->X] = ins->NN & rand();
    break;

    case OPCODE_SAVE: // 0xFX55
          for (int i = 0; i < ins->X + 1; ++i) {
            soc->memory[soc->i + i] = soc->v[i];
          }
    break;

    case OPCODE_LOAD: //0xFXi65
          for (int i = 0; i < ins->X + 1; ++i) {
            soc->v[i] = soc->memory[soc->i + i];
          }
    break;

    case OPCODE_BCD:
         soc->memory[soc->i    ] = soc->v[ins->X]/100;
         soc->memory[soc->i + 1] = (soc->v[ins->X]/10)%10;
         soc->memory[soc->i + 2] = soc->v[ins->X]%10;
    break;

    case OPCODE_SKP: // EX9E
      if (soc->key[soc->v[ins->key]]) soc->pc+=2;
    break;

    case OPCODE_SKNP: // EXA1
      if ( ! soc->key[soc->v[ins->key]]) soc->pc+=2;
    break;

    case OPCODE_WKP: // 0xFX0A
      switch (soc->kb_state) {
        case state_idle:
          soc->kb_state = state_waiting_keypress;
	  soc->pc -=2;
        break;

        case state_waiting_keypress:
	  for (uint8_t i = 0; i<16; ++i) {
            if ( soc->key[i] ) {
              soc->last_key = i;
              soc->kb_state = state_waiting_keyrelease;
            }    
          }
	  soc->pc -=2;
	break;

	case state_waiting_keyrelease:
	  {
            int done = 1;		  
            for (uint8_t i = 0; i<16; ++i) {
              if ( soc->key[i] ) {
                done = 0;
              }    
            }
	    if (done) {
              soc->kb_state = state_idle;
	      soc->v[ins->X] = soc->last_key;
	    } else {
              soc->pc -=2;
            }		    
	  }
	break;
      }
    break;

    case OPCODE_ADDI:
      soc->i += soc->v[ins->X];
    break;

    case OPCODE_SETT:
      soc->delay_timer = soc->v[ins->X];
    break;

    case OPCODE_GETT: // FX07
      soc->v[ins->X] = soc->delay_timer;
    break;

    case OPCODE_SPRITE:
    case OPCODE_SETB:
    case OPCODE_SETI:
    break;

  }	  

}
