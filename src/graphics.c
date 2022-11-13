#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "graphics.h"

void start_graphics(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture){
    *window = SDL_CreateWindow("Chip8 Emulator by kprime21", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64*SCALE, 32*SCALE, SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL | 
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_ALLOW_HIGHDPI );

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    (*texture) = SDL_CreateTexture((*renderer), 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        64, 
        32
    );
    
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 0);
    SDL_RenderClear(*renderer);
    SDL_RenderPresent(*renderer);
}

void destroy_graphics(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void buffer_graphics(chip8 *cpu, uint32_t *buffer, unsigned char* gfx,SDL_Renderer *renderer) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            uint8_t pixel = gfx[y*64+x];
            buffer[(y * 64) + x] = (0xFFFFFF00 * pixel) | 0x000000FF;
            printf("%d\n",buffer[y*64+x]);
        }
    }
}

// void draw_graphics(unsigned char* gfx, SDL_Renderer* renderer){
//     int x = 0, y = -1;
//     for(int i = 0; i < 64*32; i++){
//         if(!(i%64)){
//             y++;
//             x = 0;
//         }
//         if(gfx[i]){
//             SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         }
//         else{
//             SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
//         }
//         //scale to resolution
//         for(int j = 0; j < SCALE; j++){
//             for(int k = 0; k<SCALE; k++){
//                 SDL_RenderDrawPoint(renderer, x*SCALE+j, y*SCALE+k);
//             }
//         }
//         x++;
//     }
//     SDL_RenderPresent(renderer);

// }
void draw_graphics(uint32_t *buffer, SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_UpdateTexture(texture, NULL, buffer, 64 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}