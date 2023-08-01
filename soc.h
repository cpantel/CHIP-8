#ifndef __soc__
#define __soc__
#include <stdint.h>

#define SCREEN_SCALE   4
#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

enum KB_state {
  state_idle,
  state_waiting_keypress,
  state_waiting_keyrelease
};

struct typeSOC {
	uint16_t pc;
	uint16_t i;
	uint8_t v[16];
	uint16_t key[16];
	uint16_t stack[24];
	uint8_t  memory[0x1000];
	uint8_t  stack_pointer;
        uint32_t screen[SCREEN_WIDTH * SCREEN_HEIGHT];
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint32_t count;
	enum KB_state kb_state;
	uint8_t last_key;
};

void soc_clear_screen(struct typeSOC* soc, uint32_t color);
void soc_init(struct typeSOC* soc, char*);
int soc_step(struct typeSOC* soc, int);
void soc_update(struct typeSOC* soc);

void soc_press_key(struct typeSOC* soc, uint8_t);
void soc_release_key(struct typeSOC* soc, uint8_t);

void soc_dump_screen(struct typeSOC* soc);
void soc_dump_registers(struct typeSOC* soc);
void soc_dump_memory(struct typeSOC* soc);
void soc_dump_key(struct typeSOC* soc);


#endif // __soc
