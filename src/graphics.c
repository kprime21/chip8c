#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "graphics.h"

void start_graphics(SDL_Window** window, SDL_Renderer** renderer){
    *window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64, 32, SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL | 
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_ALLOW_HIGHDPI );

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
}

void destroy_graphics(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_graphics(unsigned char* gfx, SDL_Renderer* renderer){
    int x = 0, y = -1;
    for(int i = 0; i < 64*32; i++){
        if(!(i%64)){
            y++;
            x = 0;
        }
        if(gfx[i]){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        else{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        SDL_RenderDrawPoint(renderer, x, y);
        x++;
    }
    SDL_RenderPresent(renderer);

}