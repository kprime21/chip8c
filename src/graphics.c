#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "graphics.h"

void start_graphics(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture){
    *window = SDL_CreateWindow("Chip8 Emulator by kprime21", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH*SCALE, HEIGHT*SCALE, SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL | 
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_ALLOW_HIGHDPI );

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    (*texture) = SDL_CreateTexture((*renderer), 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        WIDTH, 
        HEIGHT
    );
    
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
}

void destroy_graphics(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void buffer_graphics(chip8 *cpu, uint32_t *buffer, unsigned char* gfx,SDL_Renderer *renderer) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            uint8_t pixel = gfx[y*WIDTH+x];
            buffer[(y * WIDTH) + x] = (0xFFFFFF00 * pixel) | 0x000000FF;
        }
    }
}

void draw_graphics(uint32_t *buffer, SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_UpdateTexture(texture, NULL, buffer, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}