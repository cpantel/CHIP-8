#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "soc.h"
#include "options.h"

void show_help() {
  printf("s: start/pause execution\n");
  printf("n: execute next step\n");
  printf("d: turn on/off debug output\n");
  printf("t: turn on/off timing output\n");
  printf("r: dump registers and pause\n");
  printf("m: dump memory and pause\n");
  printf("v: dump screen and pause\n");
  printf("k: dump key and pause\n");
  printf("h: show this help \n");
  printf("q: exit program \n");
}

int main(int argc, char **argv){
  clock_t start, end;
  double cpu_time_used;
 
  struct configType config;
  config_parse_args(argc, argv, &config);
  config_print_args(&config);
  struct typeSOC soc;
  int quit            = 0;
  int run             = ! config.start_paused;
  int debug           = config.debug_mode;
  int halted          = 0;
  int frame_delay     = config.frame_delay;
  int max_step_count  = config.max_step_count;
  int steps_by_frame  = config.steps_by_frame;
  int dump_screen     = config.dump_screen;
  int show_time       = 0;
  soc_init(&soc, config.rom_path);

  SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(SCREEN_WIDTH*SCREEN_SCALE, SCREEN_HEIGHT*SCREEN_SCALE, 0, &window, &renderer);
  SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

  while (!quit && ! halted) {
    soc.redraw = 0;
    start = clock();
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      } else if(event.type == SDL_KEYDOWN){
        if (event.key.keysym.mod & KMOD_LSHIFT ) {            
          if(event.key.keysym.sym == SDLK_s) run = ! run;
          if(event.key.keysym.sym == SDLK_n) soc_step(&soc,1);
          if(event.key.keysym.sym == SDLK_d) { debug = ! debug;          run = 0; }
          if(event.key.keysym.sym == SDLK_t) { show_time = ! show_time;   run = 0; }
          if(event.key.keysym.sym == SDLK_r) { soc_dump_registers(&soc); run = 0; }
          if(event.key.keysym.sym == SDLK_m) { soc_dump_memory(&soc);    run = 0; }
          if(event.key.keysym.sym == SDLK_v) { soc_dump_screen(&soc);    run = 0; }
          if(event.key.keysym.sym == SDLK_k) { soc_dump_key(&soc);       run = 0; }
          if(event.key.keysym.sym == SDLK_h) { show_help();              run = 0; }
          if(event.key.keysym.sym == SDLK_q) quit = 1;
        } else {
          if(event.key.keysym.sym == SDLK_1) soc_press_key(&soc, 1);
          if(event.key.keysym.sym == SDLK_2) soc_press_key(&soc, 2);
          if(event.key.keysym.sym == SDLK_3) soc_press_key(&soc, 3);
          if(event.key.keysym.sym == SDLK_4) soc_press_key(&soc, 12);
          if(event.key.keysym.sym == SDLK_q) soc_press_key(&soc, 4);
          if(event.key.keysym.sym == SDLK_w) soc_press_key(&soc, 5);
          if(event.key.keysym.sym == SDLK_e) soc_press_key(&soc, 6);
          if(event.key.keysym.sym == SDLK_r) soc_press_key(&soc, 13);
          if(event.key.keysym.sym == SDLK_a) soc_press_key(&soc, 7);
          if(event.key.keysym.sym == SDLK_s) soc_press_key(&soc, 8);
          if(event.key.keysym.sym == SDLK_d) soc_press_key(&soc, 9);
          if(event.key.keysym.sym == SDLK_f) soc_press_key(&soc, 14);
          if(event.key.keysym.sym == SDLK_z) soc_press_key(&soc, 10);
          if(event.key.keysym.sym == SDLK_x) soc_press_key(&soc, 0);
          if(event.key.keysym.sym == SDLK_c) soc_press_key(&soc, 11);
          if(event.key.keysym.sym == SDLK_v) soc_press_key(&soc, 15);
        }
      } else if(event.type == SDL_KEYUP){
        if (event.key.keysym.mod == KMOD_NONE ) {            
          if(event.key.keysym.sym == SDLK_1) soc_release_key(&soc, 1);
          if(event.key.keysym.sym == SDLK_2) soc_release_key(&soc, 2);
          if(event.key.keysym.sym == SDLK_3) soc_release_key(&soc, 3);
          if(event.key.keysym.sym == SDLK_4) soc_release_key(&soc, 12);
          if(event.key.keysym.sym == SDLK_q) soc_release_key(&soc, 4);
          if(event.key.keysym.sym == SDLK_w) soc_release_key(&soc, 5);
          if(event.key.keysym.sym == SDLK_e) soc_release_key(&soc, 6);
          if(event.key.keysym.sym == SDLK_r) soc_release_key(&soc, 13);
          if(event.key.keysym.sym == SDLK_a) soc_release_key(&soc, 7);
          if(event.key.keysym.sym == SDLK_s) soc_release_key(&soc, 8);
          if(event.key.keysym.sym == SDLK_d) soc_release_key(&soc, 9);
          if(event.key.keysym.sym == SDLK_f) soc_release_key(&soc, 14);
          if(event.key.keysym.sym == SDLK_z) soc_release_key(&soc, 10);
          if(event.key.keysym.sym == SDLK_x) soc_release_key(&soc, 0);
          if(event.key.keysym.sym == SDLK_c) soc_release_key(&soc, 11);
          if(event.key.keysym.sym == SDLK_v) soc_release_key(&soc, 15);
        }
      }
    }
    if (run) {
      for (int i=0; i <= steps_by_frame; ++i) {
        if (run) halted = soc_step(&soc, debug);
      }
    }
    if (soc.count >= max_step_count) run = 0;
    soc_update(&soc);        
       
    // Dibujar pantalla
    if (soc.redraw) {
      SDL_UpdateTexture(texture, NULL, soc.screen, SCREEN_WIDTH * sizeof(Uint32));
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
    // Esperar al próximo frame
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    if (show_time) printf("t=%f\n", cpu_time_used);
    SDL_Delay(frame_delay);
    // if (soc.count % ( steps_by_frame * frame_delay) == 0) printf("===== frame =====\n");
  }
  if (halted) {
    printf("Halted execution\n");      
    SDL_Delay(3000);
  }
  if (dump_screen) soc_dump_screen(&soc);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
