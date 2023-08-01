#ifndef __config__
#define __config__

struct configType {
   int debug_mode;
   int start_paused;
   size_t max_step_count;
   int  frame_delay;
   int steps_by_frame;
   char * rom_path;
   int dump_screen;
};

void config_parse_args(int argc, char **argv, struct configType* config);
void config_print_args(struct configType* config);

#endif
