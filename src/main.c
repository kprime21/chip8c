#include "cpu.h"
#include "graphics.h"
#include <SDL2/SDL.h>


int main(){
    SDL_Window *graphics;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Init(SDL_INIT_EVERYTHING);
    start_graphics(&graphics, &renderer,&texture);
    chip8 cpu;
    initialize(&cpu);
    load_rom(&cpu, "PONG");
    printf("first address of renderer %d\n", &renderer );
    while(cpu.run_flag){
        cycle(&cpu);
        cpu.key[4] = 1;
        if(cpu.draw_flag){
            draw_graphics(gfx,renderer);
            cpu.draw_flag = 0;
        }      
    }    
}