#include "cpu.h"
#include "graphics.h"
#include <SDL2/SDL.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#define CLOCK_CYCLE 8
#define FPS 60

long long millis() {
  struct timeval time;
  gettimeofday(&time, NULL);
  long long s1 = (long long)(time.tv_sec) * 1000;
  long long s2 = (time.tv_usec / 1000);
  return s1 + s2;
}

int main(){
    SDL_Window *graphics;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Init(SDL_INIT_EVERYTHING);
    start_graphics(&graphics, &renderer,&texture);
    chip8 cpu;
    initialize(&cpu);
    load_rom(&cpu, "PONG");
    //make clock run 8 instructions per 1/60fps
    long long execTime = millis();
    const long long desiredDelta = 1000/FPS;
    

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
                draw_graphics(gfx,renderer);
                cpu.draw_flag = 0;
        }
        execTime = millis();      
    }    
}