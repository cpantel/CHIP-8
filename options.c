#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include "options.h"

void config_print_args(struct configType* config);

void config_print_args(struct configType* config) {
  printf("Debug Mode    : %d\n", config->debug_mode);
  printf("Start Paused  : %d\n", config->start_paused);
  printf("Max Step Count: %zu\n", config->max_step_count);
  printf("Frame Delay   : %d\n", config->frame_delay);
  printf("Steps by Frame: %d\n", config->steps_by_frame);
  printf("ROM path      : %s\n", config->rom_path);
  printf("Dump Screen   : %d\n", config->dump_screen);
}

void config_parse_args(int argc, char **argv, struct configType* config) {

  config->debug_mode     = 0;
  config->start_paused   = 0;
  config->max_step_count = INT_MAX;
  config->frame_delay    = 16;
  config->steps_by_frame = 8;
  config->rom_path       = 0;
  config->dump_screen    = 0;

// Adapted from https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
  int c;

  while (1) {
    static struct option long_options[] = {
      {"debug_mode"  ,   no_argument,       0, 'D'},
      {"start_paused"  , no_argument,       0, 'P'},
      {"dump_screen"   , no_argument,       0, 'S'},
      {"max_step_count", required_argument, 0, 'm'},
      {"steps_by_frame", required_argument, 0, 's'},
      {"frame_delay",    required_argument, 0, 'f'},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    c = getopt_long (argc, argv, "DPSm:s:f:", long_options, &option_index);

    // Detect the end of the options.
    if (c == -1)
      break;
    switch (c) {
      case 0:
        // If this option set a flag, do nothing else now. 
        if (long_options[option_index].flag != 0) break;
        printf ("option %s", long_options[option_index].name);
        if (optarg) printf (" with arg %s", optarg);
        printf ("\n");
      break;
      case 'D':
        config->debug_mode = 1;
      break;
      case 'S':
        config->dump_screen = 1;
      break;
      case 'P':
        config->start_paused = 1;
      break;
      case 'm':
        config->max_step_count = atoi(optarg);
      break;
      case 's':
        config->steps_by_frame = atoi(optarg);
      break;
      case 'f':
        config->frame_delay = atoi(optarg);
      break;
      case '?':
        // getopt_long already printed an error message.
      break;
      default:
        abort ();
    }
  }
  if (optind < argc) {
    config->rom_path = argv[optind--];
  }
}

