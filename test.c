#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soc.h"
#include "api.h"
#include "instruction.h"

void assert_equal(uint32_t expected, uint32_t got, char * msg) {
  if (expected != got) {
    printf("\n%s, expected: %x got: %x\n", msg, expected, got);
  } else {
    printf(".");
  }	  
}

void assert_equal_memory(char* expected, char* got, char * msg) {
  if (memcmp(expected, got, 3) != 0 ) {
    printf("\n%s, expected: %s got: %s\n", msg, expected, got);
  } else {
    printf(".");
  }    
}




/*void test_(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->pc = 0;
  ins->fetch = 0x;
  predecode(ins);
  api(soc, ins, OPCODE_);
  assert_equal(0, , " bad");
}
*/

void test_RND(struct typeSOC* soc, struct typeInstruction* ins) {
  ins->fetch = 0xCAAA;
  predecode(ins);
  srand(4);
  api(soc, ins, OPCODE_RND);
  assert_equal(0x88,soc->v[0xA],"RND bad with seed 4");
}

void test_SKP(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->pc = 0x200;
  soc->key[4]=0;
  ins->fetch = 0xE493;
  predecode(ins),
  api(soc, ins, OPCODE_SKP);
  assert_equal(0x200, soc->pc, "SKP: bad jump");
  soc->key[4]=1;
  api(soc, ins, OPCODE_SKP);
  assert_equal(0x202, soc->pc, "SKP: missing jump");
}


void test_SKNP(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->pc = 0x200;
  soc->key[4]=1;
  ins->fetch = 0xE493;
  predecode(ins),
  api(soc, ins, OPCODE_SKNP);
  assert_equal(0x200, soc->pc, "SKNP: bad jump");
  soc->key[4]=0;
  api(soc, ins, OPCODE_SKNP);
  assert_equal(0x202, soc->pc, "SKNP: missing jump");
}



void test_soc_press_key(struct typeSOC* soc){
  soc->key[2] = 0;	
  soc_press_key(soc,2);
  assert_equal(1,soc->key[2], "soc_press_key: bad value");
}



void test_SETT_soc_update_GETT(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[0xa] = 0x18;
  soc->v[0xb] = 0x0;
  ins->fetch = 0xa15;
  predecode(ins);
  api(soc, ins, OPCODE_SETT);
  soc_update(soc);
  soc_update(soc);
  soc_update(soc);
  ins->fetch = 0xb07;
  predecode(ins);
  api(soc, ins, OPCODE_GETT);
  assert_equal(0x15, soc->v[0xb], "SETT_update_GETT bad value");
}

void test_soc_update(struct typeSOC* soc) {
  soc->delay_timer = 0x12;
  soc_update(soc);
  assert_equal(0x11, soc->delay_timer, "soc_update: bad value");
}

void test_GETT(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[3] = 0;	
  soc->delay_timer = 0x14;
  ins->fetch =0xf307;
  predecode(ins);
  api(soc, ins, OPCODE_GETT);
  assert_equal(0x14, soc->v[3], "GETT: bad value");
}

void test_SETT(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->delay_timer = 0;
  ins->fetch =0xf315;
  soc->v[3] = 0x18;
  predecode(ins);
  api(soc, ins, OPCODE_SETT);
  assert_equal(0x18, soc->delay_timer, "SETT: bad value");
}

void test_ADDI(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->i = 7;
  soc->v[4] = 6;
  ins->fetch = 0x41E;
  predecode(ins);
  api(soc, ins, OPCODE_ADDI);
  assert_equal(0xd, soc->i, "ADDI bad value");
}

void test_BCD(struct typeSOC* soc, struct typeInstruction* ins) {
  ins->fetch = 0xf333;
  soc->i = 14;
  soc->memory[14] = 'a';
  soc->memory[14 + 1] = 'a';
  soc->memory[14 + 2] = 'a';
  soc->memory[14 + 3] = 'f';
  predecode(ins);

  char expected1[]= {0,0,1};
  char expected2[]= {0,2,3};
  char expected3[]= {2,3,4};
  soc->v[3]=1;
  api(soc, ins, OPCODE_BCD);
  assert_equal_memory(expected1, (char*)&(soc->memory)[soc->i], "BCD 001: bad value");
  assert_equal('f', soc->memory[14 + 3], "BCD 001: bad guard");

  soc->v[3]=23;
  api(soc, ins, OPCODE_BCD);
  assert_equal_memory(expected2, (char*)&(soc->memory)[soc->i], "BCD 023: bad value");
  assert_equal('f', soc->memory[14 + 3], "BCD 023: bad guard");

  soc->v[3]=234;
  api(soc, ins, OPCODE_BCD);
  assert_equal_memory(expected3, (char*)&(soc->memory)[soc->i], "BCD 234: bad value");
  assert_equal('f', soc->memory[14 + 3], "BCD 234: bad guard");
}

void test_LOAD(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->i        =3;  
  soc->memory[3]=0;	
  soc->memory[4]=1;	
  soc->memory[5]=2;	
  soc->memory[6]=3;	
  soc->memory[7]=4;	
  soc->memory[8]=5;	
  soc->memory[9]=6;
  soc->memory[0xa]=7;
  soc->v[7] = 0xf;
  ins->fetch = 0X9665;
  predecode(ins);
  api(soc, ins, OPCODE_LOAD);
  assert_equal(0, soc->v[0], "LOAD 0; bad value");
  assert_equal(1, soc->v[1], "LOAD 1; bad value");
  assert_equal(2, soc->v[2], "LOAD 2; bad value");
  assert_equal(3, soc->v[3], "LOAD 3; bad value");
  assert_equal(4, soc->v[4], "LOAD 4; bad value");
  assert_equal(5, soc->v[5], "LOAD 5; bad value");
  assert_equal(6, soc->v[6], "LOAD 6; bad value");
  assert_equal(0xf, soc->v[7], "LOAD 6; bad guard");

}
void test_SAVE(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->i    = 6;  
  soc->v[0] = 0;
  soc->v[1] = 1; 
  soc->v[2] = 2; 
  soc->v[3] = 3; 
  soc->v[4] = 4; 
  soc->v[5] = 5; 
  soc->v[6] = 6; 
  soc->v[7] = 7;
  soc->memory[0xe] = 0xff;
  ins->fetch = 0xf755; 
  predecode(ins);
  api(soc, ins, OPCODE_SAVE);
  assert_equal(0, soc->memory[6], "SAVE 0: bad value");
  assert_equal(1, soc->memory[7], "SAVE 1: bad value");
  assert_equal(2, soc->memory[8], "SAVE 2: bad value");
  assert_equal(3, soc->memory[9], "SAVE 3: bad value");
  assert_equal(4, soc->memory[0xa], "SAVE 4: bad value");
  assert_equal(5, soc->memory[0xb], "SAVE 5: bad value");
  assert_equal(6, soc->memory[0xc], "SAVE 6: bad value");
  assert_equal(7, soc->memory[0xd], "SAVE 7: bad value");
  assert_equal(0xff, soc->memory[0xe], "SAVE: bad guard");
}



void test_SHL(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[3]=4;
  ins->fetch = 0x8306;
  predecode(ins);
  api(soc, ins, OPCODE_SHL);
  assert_equal(8, soc->v[3], "SHL 1: bad shift");
  assert_equal(0, soc->v[0xf], "SHL 1: bad extra carry");

  soc->v[3]=0xf0;
  api(soc, ins, OPCODE_SHL);
  assert_equal(0xe0, soc->v[3], "SHL 2: bad shift");
  assert_equal(1, soc->v[0xf], "SHL 2: bad missing carry");
}
void test_SHR(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[3]=4;
  ins->fetch = 0x8346;
  predecode(ins);
  api(soc, ins, OPCODE_SHR);
  assert_equal(2, soc->v[3], "SHR 1: bad shift");
  assert_equal(0, soc->v[0xf], "SHR 1: bad extra carry");

  soc->v[3]=5;
  api(soc, ins, OPCODE_SHR);
  assert_equal(2, soc->v[3], "SHR 2: bad shift");
  assert_equal(1, soc->v[0xf], "SHR 2: bad missing carry");

  soc->v[0xF] = 0x3d;
  ins->fetch = 0x8ff6;
  api(soc, ins, OPCODE_SHR);
  assert_equal(0x1, soc->v[0xF], "SHR 3: bad vF treatment");

}

void test_SUBI(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[3] = 0xff;
  soc->v[4] = 0x01;
  ins->fetch = 0x8437;
  predecode(ins);

  api(soc, ins, OPCODE_SUBI);
  assert_equal(0xfe, soc->v[4], "SUBI 1: bad result");
  assert_equal(0x01, soc->v[0xf], "SUBI 1: bad extra carry");

  soc->v[3] = 0xfe;
  soc->v[4] = 0xff;
  api(soc, ins, OPCODE_SUBI);
  assert_equal(0xff, soc->v[4], "SUBI 2: bad result");
  assert_equal(0x00, soc->v[0xf], "SUBI 2: bad missing carry");
}



void test_SUB(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[3] = 0xff;
  soc->v[4] = 0x01;
  ins->fetch = 0x8345;
  predecode(ins);

  api(soc, ins, OPCODE_SUB);
  assert_equal(0xfe, soc->v[3], "SUB 1: bad result");
  assert_equal(0x01, soc->v[0xf], "SUB 1: bad extra carry");

  soc->v[3] = 0xfe;
  soc->v[4] = 0xff;
  api(soc, ins, OPCODE_SUB);
  assert_equal(0xff, soc->v[3], "SUB 2: bad result");
  assert_equal(0x00, soc->v[0xf], "SUB 2: bad missing carry");
}


void test_ADDC(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[3] = 0xff;
  soc->v[4] = 0x01;
  ins->fetch = 0x8344;
  predecode(ins);

  api(soc, ins, OPCODE_ADDC);
  assert_equal(0x00, soc->v[3], "ADDC 1: bad result");
  assert_equal(0x01, soc->v[0xf], "ADDC 1: bad missing carry");

  soc->v[3] = 0xfe;
  api(soc, ins, OPCODE_ADDC);
  assert_equal(0xff, soc->v[3], "ADDC 2: bad result");
  assert_equal(0x00, soc->v[0xf], "ADDC 2: bad extra carry");
}

void test_OR(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[3] = 0x03;
  soc->v[4] = 0x2a;
  ins->fetch = 0x8341;
  predecode(ins);
  api(soc, ins, OPCODE_OR);
  assert_equal(0x2b, soc->v[3], "OR: bad result");
}
void test_AND(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[0] = 0xf0;
  soc->v[7] = 0x2a;
  ins->fetch = 0x8072;
  predecode(ins);
  api(soc, ins, OPCODE_AND);
  assert_equal(0x20, soc->v[0], "AND: bad result");
}
void test_XOR(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[8] = 0xff;
  soc->v[2] = 0x2a;
  ins->fetch = 0x8823;
  predecode(ins);
  api(soc, ins, OPCODE_XOR);
  assert_equal(0xd5, soc->v[8], "XOR: bad result");
}



void test_CP(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[7] = 0x1;
  soc->v[5] = 0x2a;
  ins->fetch = 0x8750;
  predecode(ins);
  api(soc, ins, OPCODE_CP);
  assert_equal(0x2a, soc->v[7], "CP: bad ");
}



void test_CALL_RET(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->pc    =  0x2a2;
  ins->fetch = 0x2202;
  soc->stack_pointer = 0;
  soc->stack[soc->stack_pointer] = 0;
 
  predecode(ins);
  api(soc, ins, OPCODE_CALL);
  ins->fetch = 0x00EE;
  api(soc, ins, OPCODE_RET);
  assert_equal(0x2a2, soc->pc , "CALL_RET: bad pc");
  assert_equal(0, soc->stack_pointer, "CALL_RET: bad stackpointer");
}

void test_CALL(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->pc    =  0x2a2;
  ins->fetch = 0x2202;
  soc->stack_pointer = 0;
  soc->stack[soc->stack_pointer] = 0;
 
  predecode(ins);
  api(soc, ins, OPCODE_CALL);
  assert_equal(0x202, soc->pc , "CALL: bad pc");
  assert_equal(1, soc->stack_pointer, "CALL: bad stackpointer");
  assert_equal(0x2a2, soc->stack[soc->stack_pointer-1], "CALL: bad stack pointer");
}

void test_RET(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->stack[0] = 0x123;
  soc->stack[1] = 0x456;
  soc->stack[2] = 0x789;

  ins->fetch = 0x00EE;
  predecode(ins);

  soc->stack_pointer = 1;
  api(soc, ins, OPCODE_RET);
  assert_equal(0, soc->stack_pointer, "RET 1: bad stackpointer");
  assert_equal(0x123, soc->pc , "RET 1: bad pc");

  soc->stack_pointer = 2;
  api(soc, ins, OPCODE_RET);
  assert_equal(1, soc->stack_pointer, "RET 2: bad stackpointer");
  assert_equal(0x456, soc->pc , "RET 2: bad pc");
  assert_equal(0x123, soc->stack[soc->stack_pointer-1], "RET 2: bad stack pointer");
}

void test_SEQ(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[6]  =   0x2b;
  soc->pc    =  0x22e;
  ins->fetch = 0x362b;
  predecode(ins);
  api(soc, ins, OPCODE_SEQ);
  assert_equal(0x230, soc->pc, "SEQ: wrong when equal");

  soc->v[6]  =   0x2b;
  soc->pc    =  0x22e;
  ins->fetch = 0x362a;
  predecode(ins);
  api(soc, ins, OPCODE_SEQ);
  assert_equal(0x22e, soc->pc, "SEQ: wrong when not equal");
}

void test_SNE(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[6]  =   0x2b;
  soc->pc    =  0x22e;
  ins->fetch = 0x362b;
  predecode(ins);
  api(soc, ins, OPCODE_SNE);
  assert_equal(0x22e, soc->pc, "SNE: wrong when equal");

  soc->v[6]  =   0x2b;
  soc->pc    =  0x22e;
  ins->fetch = 0x362a;
  predecode(ins);
  api(soc, ins, OPCODE_SNE);
  assert_equal(0x230, soc->pc, "SNE: wrong when not equal");
}

void test_SCNE(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[7] = 3;
  soc->v[8] = 3;
  soc->v[9] = 4;

  soc->pc  = 0x200;
  ins->fetch = 0x9780;
  predecode(ins);
  api(soc,ins,OPCODE_SCNE);
  assert_equal(0x200, soc->pc, "SCEQ: wrong when equal");

  soc->pc  = 0x200;
  ins->fetch = 0x9790;
  predecode(ins);
  api(soc,ins,OPCODE_SCNE);
  assert_equal(0x202, soc->pc, "SCEQ: wrong when not equal");
}

void test_SCEQ(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->v[7] = 3;
  soc->v[8] = 3;
  soc->v[9] = 4;

  soc->pc  = 0x200;
  ins->fetch = 0x9780;
  predecode(ins);
  api(soc,ins,OPCODE_SCEQ);
  assert_equal(0x202, soc->pc, "SCEQ: wrong when equal");

  soc->pc  = 0x200;
  ins->fetch = 0x9790;
  predecode(ins);
  api(soc,ins,OPCODE_SCEQ);
  assert_equal(0x200, soc->pc, "SCEQ 1: wrong when not equal");

  soc->v[5]=0x2a;
  soc->v[6]=0x2b;
  soc->pc = 0x252;
  ins->fetch = 0x5560;
  predecode(ins);
  api(soc,ins,OPCODE_SCEQ);
  assert_equal(0x252, soc->pc, "SCEQ 2: wrong when not equal");
}

void test_ADD(struct typeSOC* soc, struct typeInstruction* ins) {
  ins->fetch =0x7B05;
  predecode(ins);
  soc->v[0xB]=0x15;
  api(soc,ins,OPCODE_ADD);
  assert_equal(0x1a, soc->v[0xb], "ADD: failed addition");
}

void test_LDI(struct typeSOC* soc, struct typeInstruction* ins) {
  ins->fetch = 0xA461;
  predecode(ins);
  api(soc,ins,OPCODE_LDI);
  if (soc->i != 0x461) printf("LDI: failed to load index %X\n", soc->i);
}

void test_LD(struct typeSOC* soc, struct typeInstruction* ins) {
  ins->fetch = 0x6832;
  predecode(ins);
  api(soc,ins,OPCODE_LD);
  if ( soc->v[8] != 0x32) printf("LD: failed register load: %X\n", soc->v[8]);
}

void test_JMP(struct typeSOC* soc, struct typeInstruction* ins) {
  ins->fetch = 0x1208;
  predecode(ins);
  api(soc,ins,OPCODE_JMP);
  if (soc->pc != 0x208) printf("JMP: failed pc update %X\n", soc->pc);  
}

void test_JMPO(struct typeSOC* soc, struct typeInstruction* ins) {
  soc->pc = 0x200;
  soc->v[0]=0x12;	
  ins->fetch = 0xA002;
  predecode(ins);
  api(soc,ins,OPCODE_JMPO);
  assert_equal(0x14, soc->pc, "JMPO: failed pc update");  
}


int main(int argc, char **args){
    struct typeSOC soc;
    struct typeInstruction ins;
    test_RET(&soc,&ins);
    test_JMP(&soc,&ins);
    test_LD(&soc,&ins);
    test_LDI(&soc,&ins);
    test_ADD(&soc,&ins);
    test_SNE(&soc,&ins);
    test_SEQ(&soc,&ins);
    test_SCEQ(&soc,&ins);
    test_SCNE(&soc,&ins);
    test_CALL(&soc,&ins);
    test_OR(&soc,&ins);
    test_AND(&soc,&ins);
    test_XOR(&soc,&ins);
    test_ADDC(&soc,&ins);
    test_SUB(&soc,&ins);
    test_SHR(&soc,&ins);
    test_SUBI(&soc,&ins);
    test_JMPO(&soc,&ins);
    test_SHL(&soc,&ins);
    test_LOAD(&soc,&ins);
    test_SAVE(&soc,&ins);
    test_BCD(&soc,&ins);
    test_ADDI(&soc,&ins);
    test_SETT(&soc,&ins);
    test_GETT(&soc,&ins);
    test_SETT_soc_update_GETT(&soc,&ins);
    test_SKNP(&soc,&ins);
    test_SKP(&soc,&ins);
    test_RND(&soc,&ins);
    test_soc_update(&soc);
    test_soc_press_key(&soc);
//    test_(&soc,&ins);
//
    printf("!\n");
}
