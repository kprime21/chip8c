#include <SDL2/SDL.h>
#include "cpu.h"
#define SCALE 16

//double pointers because we are changing what they point to
void start_graphics(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **sdl_texture);
void destroy_graphics(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture);
void buffer_graphics(chip8 *cpu, uint32_t *buffer, unsigned char* gfx, SDL_Renderer *renderer);
void draw_graphics(uint32_t *buffer, SDL_Renderer *renderer,SDL_Texture *texture);
