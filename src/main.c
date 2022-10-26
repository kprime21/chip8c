#include "cpu.h"
#include "graphics.h"
#include <SDL2/SDL.h>


int main(){
    //initalize a black screen first

    
    SDL_Window *graphics;
    SDL_Renderer *renderer;
    start_graphics(graphics, renderer);
    chip8 cpu;
    initialize(&cpu);
    load_rom(&cpu, "PONG");
    for(int i = 0; i < 246; i+=2){
        cycle(&cpu);
    }
    
    
    
}