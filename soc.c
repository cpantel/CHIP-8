#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "soc.h"
#include "api.h"

void soc_set_pixel(struct typeSOC * soc, uint8_t x, uint8_t y, uint32_t value) {
    soc->screen[y * SCREEN_WIDTH + x] = value;
}

uint32_t soc_get_pixel(struct typeSOC* soc, uint8_t x, uint8_t y) {
    return soc->screen[y * SCREEN_WIDTH + x];
}

void unimplemented(uint32_t count, uint16_t fetch){
  printf("UNIMPLEMENTED INSTRUCTION: %04x @ %d\n", fetch, count);
  exit(1);          
}

void unknown(uint32_t count, uint16_t fetch){
  printf("UNKNOWN INSTRUCTION: %04x @ %d\n", fetch, count);
  exit(3);          
}

void invalid(uint32_t count, uint16_t fetch){
  printf("INVALID INSTRUCTION: %04x @ %d\n", fetch, count);
  exit(2);          
}

void soc_clear_screen(struct typeSOC* soc, uint32_t color) {
   memset(soc->screen, color, sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);
}

void soc_update(struct typeSOC* soc) { 
    if(soc->delay_timer > 0) soc->delay_timer--; 
    if(soc->sound_timer > 0) soc->sound_timer--; 
} 

void soc_press_key(struct typeSOC* soc, uint8_t pos) {
  soc->key[pos] = 1;
}

void soc_release_key(struct typeSOC* soc, uint8_t pos) {
  soc->key[pos] = 0;
}

void dump_registers(struct typeSOC* soc, struct typeInstruction* ins, int debug) {
  if (debug) {
    printf("Count=%04d PC=%03x Opcode=%04x V[", soc->count, soc->pc, ins->fetch);
    for (int i=0; i<16; ++i) {
      printf("%X=%02x ", i, soc->v[i]);        
    }
    
    printf("] IDX=%03x SP=%02x S[SP]=%03x\n", soc->i, soc->stack_pointer, soc->stack[soc->stack_pointer]);
  }      
}    

int soc_step(struct typeSOC* soc, int debug) {
  struct typeInstruction ins;
  fetch(soc,&ins);
  dump_registers(soc, &ins, debug);
  if (ins.opcode == 1 && soc->pc == ins.NNN) {
     return 1;
  }

  soc->pc+=2;
  soc->count++;

  switch (ins.opcode) {
    case 0x0: // CLS
      switch(ins.NNN) {
        case 0x0E0:
          api(soc, &ins, OPCODE_CLS);
          if (debug) printf("  CLS\n");
    break;  

        case 0x0EE: // RET
          api(soc,&ins, OPCODE_RET);
          if (debug) printf("  RET   pc=%03X sp=%03X stack_top=%03X\n", soc->pc, soc->stack_pointer, soc->stack[soc->stack_pointer]) ;
    break;

        default:    
          invalid(soc->count, ins.fetch);
        break;
      }
    break;

    case 0x1: // JMP
      api(soc,&ins, OPCODE_JMP);
      if (debug) printf("  JMP   pc=%03X\n", soc->pc);
    break;  

    case 0x2: // CALL
      api(soc,&ins, OPCODE_CALL);
      if (debug) printf("  CALL   pc=%03X sp=%03X stack_top=%03X\n", soc->pc, soc->stack_pointer, soc->stack[soc->stack_pointer -1]) ;
    break;  

    case 0x3: // SEQ
      api(soc,&ins, OPCODE_SEQ);
      if (debug) printf("  SEQ   pc=%03X\n", soc->pc);
    break;

    case 0x4: // SNE
      api(soc,&ins, OPCODE_SNE);
      if (debug) printf("  SNE  pc=%03X\n", soc->pc);
    break;

    case 0x5: // SCEQ
      api(soc,&ins, OPCODE_SCEQ);
      if (debug) printf(" SCEQ  pc=%03X\n", soc->pc);
    break;

    case 0x6: // LD
      api(soc,&ins, OPCODE_LD);
      if (debug) printf("  LD    v:[%X]+=%02X\n", ins.X, soc->v[ins.X]);
    break;

    case 0x7: // ADD
      api(soc,&ins, OPCODE_ADD);
      if (debug) printf(" ADD  v:[%X]+=%02X\n", ins.X, soc->v[ins.X]);
    break;

    case 0x8: // 
     switch(ins.opcode2) { 
        case 0x0: // CPY
          api(soc,&ins, OPCODE_CP);
          if (debug) printf(" ###  v:[%X]=%02X\n", ins.X, soc->v[ins.X]);
    break;

        case 0x1: // OR
          api(soc,&ins, OPCODE_OR);
          if (debug) printf(" ### v:[%X]+=%02X\n", ins.X, soc->v[ins.X]);
    break;

        case 0x2: // AND
          api(soc,&ins, OPCODE_AND);
          if (debug) printf(" ### v:[%X]+=%02X\n", ins.X, soc->v[ins.X]);
    break;

        case 0x3: // XOR
          api(soc,&ins, OPCODE_XOR);
          if (debug) printf(" ### v:[%X]+=%02X\n", ins.X, soc->v[ins.X]);
    break;

        case 0x4: { // ADDC
          api(soc,&ins, OPCODE_ADDC);
          if (debug) printf(" ### v:[%X]+=%02X v:[F]=%02X\n", ins.X, soc->v[ins.X], soc->v[0xF]);
        }
        break;

        case 0x5: // SUB
          api(soc,&ins, OPCODE_SUB);
          if (debug) printf(" ### v:[%X]+=%02X v:[F]=%02X\n", ins.X, soc->v[ins.X], soc->v[0xF]);
    break;

    case 0x6: // SHR
          api(soc,&ins, OPCODE_SHR);
          if (debug) printf(" ### v:[%X]+=%02X v:[F]=%02X\n", ins.X, soc->v[ins.X], soc->v[0xF]);
    break;

        case 0x7: // SUBI
          api(soc,&ins, OPCODE_SUBI);
          if (debug) printf(" ### v:[%X]+=%02X v:[F]=%02X\n", ins.X, soc->v[ins.X], soc->v[0xF]);
    break;

    case 0xE: // SHL
          api(soc,&ins, OPCODE_SHL);
          if (debug) printf(" ### v:[%X]+=%02X v:[F]=%02X\n", ins.X, soc->v[ins.X], soc->v[0xF]);
    break;
    
        default:
          unimplemented(soc->count, ins.fetch);      
        break;
      }
    break;

    case 0x9: // SCEQ
      api(soc, &ins, OPCODE_SCNE);
      if (debug) printf("  ###  pc=%03X\n", soc->pc);
    break;

    case 0xA: // LDI
      api(soc, &ins, OPCODE_LDI);
      if (debug) printf(" LDI  i: %04X\n", soc->i);
    break;

    case 0xB: // JMPO 
      api(soc, &ins, OPCODE_JMPO);
      if (debug) printf(" JMPO\n");
    break;

    case 0xC: // RND
      unimplemented(soc->count, ins.fetch);      
      api(soc, &ins, OPCODE_RND);
      if (debug) printf(" RND  v:[%X] = %02X\n", ins.X, soc->v[ins.X]);
    break;

    case 0xD: // SPRITE
      api(soc, &ins, OPCODE_SPRITE);
      if (debug) printf("  SPRITE\n");
    break;

    case 0xE:
      switch (ins.NN) {
        case 0x9e: // SKP
          api(soc, &ins, OPCODE_SKP);
        break;

        case 0xA1: // SKNP
          api(soc, &ins, OPCODE_SKNP);
        break;

        default:
          unknown(soc->count, ins.fetch);
        break;
      }
    break;

    case 0xF: // 
      switch (ins.NN) {
        case 0x07: // GETT
          api(soc, &ins, OPCODE_GETT);
        break;

        case 0x0A: // WKP
          api(soc, &ins, OPCODE_WKP);
        break;      

        case 0x15:  // SETT
          api(soc, &ins, OPCODE_SETT);
        break;

        case 0x1E:  // ADDI
          api(soc, &ins, OPCODE_ADDI);
        break;       

        case 0x29: // xxxx
          // index = font[Vx] : index = Vx * 5;
          unimplemented(soc->count, ins.fetch);
        break;

        case 0x33:  // BCD
          api(soc, &ins, OPCODE_BCD);
        break;

        case 0x55:  // SAVE
          api(soc, &ins, OPCODE_SAVE);
        break;   

        case 0x65:  // LOAD 
          api(soc, &ins, OPCODE_LOAD);
        break;
         
        default:
          unimplemented(soc->count, ins.fetch);      
        break;
      }
    break;

    default:
      unknown(soc->count, ins.fetch);
    break;  
  }
  return 0;
}


void soc_init(struct typeSOC* soc, char* filename) {
   soc->count         =     0;
   soc->pc            = 0x200;
   soc->i             =   0x0;
   soc->stack_pointer =   0x0;
   soc->kb_state      = state_idle;
   soc->last_key      =     0;
   soc_clear_screen(soc,0xFFFFFFFF);
   memset(soc->memory, 0, sizeof(uint8_t) * 0x1000);
   memset(soc->v,      0, sizeof(uint16_t) * 16 );
   memset(soc->key,    0, sizeof(uint8_t) * 8 );
   
   FILE *input = fopen(filename, "rb");
   size_t result = fread( (void *) &( soc->memory[0x200] ) , 1,  0xE00, input);
   printf("Read %zu bytes\n", result);
   fclose(input);

   uint8_t font[80] = {
     0x60, 0xa0, 0xa0, 0xa0, 0xc0,
     0x40, 0xc0, 0x40, 0x40, 0xe0,
     0xc0, 0x20, 0x40, 0x80, 0xe0,
     0xc0, 0x20, 0x40, 0x20, 0xc0,
     0x20, 0xa0, 0xe0, 0x20, 0x20,
     0xe0, 0x80, 0xc0, 0x20, 0xc0,
     0x40, 0x80, 0xc0, 0xa0, 0x40,
     0xe0, 0x20, 0x60, 0x40, 0x40,
     0x40, 0xa0, 0x40, 0xa0, 0x40,
     0x40, 0xa0, 0x60, 0x20, 0x40,
     0x40, 0xa0, 0xe0, 0xa0, 0xa0,
     0xc0, 0xa0, 0xc0, 0xa0, 0xc0,
     0x60, 0x80, 0x80, 0x80, 0x60,
     0xc0, 0xa0, 0xa0, 0xa0, 0xc0,
     0xe0, 0x80, 0xc0, 0x80, 0xe0,
     0xe0, 0x80, 0xc0, 0x80, 0x80
  };

  for(int i=0; i<80; ++i) {
    soc->memory[i] = font[i];
  }
}
void print_line(char* start, char* center, char* end) {
  printf("   %s%s", start, center);    
  for (int x=1; x < SCREEN_WIDTH; ++x) {
    printf("%s",center);
  }      
  printf("%s\n",end);
}

void soc_dump_screen(struct typeSOC* soc) {

  printf("    0");    
  for (int x=1; x < SCREEN_WIDTH; ++x) {
    if (x %10 == 0) {
      printf("+");
    } else {
      printf("%d", x %10);
    }
  }      
  printf("\n");
  print_line("\u2554", "\u2550", "\u2557");  
 
  for (int y=0; y< SCREEN_HEIGHT; ++y) {
    printf("%02d \u2551", y);      
    for (int x=0; x < SCREEN_WIDTH; ++x) {
      if (soc_get_pixel(soc, x, y)) {
        // https://en.wikipedia.org/wiki/Box-drawing_character#Unicode
        printf("\u2588");
      } else {
        printf(" ");
      }
    }
    printf("\u2551\n");
  }
  print_line("\u255A", "\u2550", "\u255D");  
}

void soc_dump_key(struct typeSOC* soc) {
  for (int i=0; i<16; ++i) {
    printf("key %x : %d\n", i, soc->key[i]);
  }
  printf("last key: %x\n", soc->last_key);
}

void soc_dump_registers(struct typeSOC* soc) {

  printf("Program Counter : %X\n", soc->pc);
  printf("Index Register  : %X\n", soc->i);
  printf("Stack Pointer   : %X\n", soc->stack_pointer);
  for (int i=0; i< 4; ++i) {
    for (int j=0; j<4; ++j) {
      int t=i+j*4;    
      printf("Register V%X     : %X    |", t, soc->v[t]);
    }
    printf("\n");
  }
}
void soc_dump_memory(struct typeSOC* soc) { 
  for (int row = 0; row < ( 0x1000 / 16); ++ row) {
    printf("%08d  ", row * 16);
    for (int col = 0; col < 16; ++ col) {
      printf("%02x ", soc->memory[row*16 + col]);
    if (col == 8 ) printf(" ");
    }
    printf("\n");
  }
}

