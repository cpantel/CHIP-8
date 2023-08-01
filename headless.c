#include <stdint.h>
#include "soc.h"

int main(int argc, char **argv){
    struct typeSOC soc;
    soc_init(&soc, "ROM/5-quirks.ch8");
    while ( 1 ) {
	soc_step(&soc, 1);
        if(soc.delay_timer > 0) soc.delay_timer--;
	if(soc.sound_timer > 0) soc.sound_timer--;
    }
    soc_dump_screen(&soc);
    return 0;
}
