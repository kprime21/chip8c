#include "cpu.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#define CLOCK_CYCLE 9
#define FPS 60


long long millis() {
  struct timeval time;
  gettimeofday(&time, NULL);
  long long s1 = (long long)(time.tv_sec) * 1000;
  long long s2 = (time.tv_usec / 1000);
  return s1 + s2;
}

int main(){
    printf("\a");
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Init(SDL_INIT_EVERYTHING);
    start_graphics(&window, &renderer, &texture);
    chip8 cpu;
    initialize(&cpu);
    load_rom(&cpu, "PONG.ch8");
    //make clock run 8 instructions per 1/60s
    long long execTime = millis();
    const long long desiredDelta = 1000/FPS;
    long long frames = millis();
    uint32_t *pixel_buffer = malloc((WIDTH * HEIGHT) * sizeof(uint32_t));
    
    while(cpu.run_flag){
        for(int i =0; i<CLOCK_CYCLE;i++)
        {
            cycle(&cpu);
        }
        long long delta = millis() - execTime;
        if(delta < desiredDelta){
            usleep((desiredDelta-delta)*1000);
        }
        if(cpu.draw_flag){
                buffer_graphics(&cpu, pixel_buffer, gfx, renderer);
                draw_graphics(pixel_buffer, renderer, texture);
                frames = millis();
                cpu.draw_flag = 0;
        }

        do {
        process_key(&cpu);
        } while (cpu.pause_flag && cpu.run_flag);

        dec_timer(&cpu);
        execTime = millis();      
    }  
    destroy_graphics(window, renderer, texture);  
}
