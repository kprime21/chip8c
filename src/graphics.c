#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "graphics.h"

//graphics here




void start_graphics(SDL_Window* window, SDL_Renderer* renderer){

   
    
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64, 32, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void destroy_graphics(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void draw_graphics(unsigned char* gfx, SDL_Renderer* renderer){
 

}

// const int fps = 120;
// const int desiredDelta = 1000/fps;
            //     //start time
    //     int startTime = SDL_GetTicks();
   //         //set FPS to 60
    //         int delta = SDL_GetTicks() - startTime;
    //         if(delta < desiredDelta){
    //             SDL_Delay(desiredDelta - delta);
    //         }