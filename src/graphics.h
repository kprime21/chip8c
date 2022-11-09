#include <SDL2/SDL.h>
#define SCALE 8

//double pointers because we are changing what they point to
void start_graphics(SDL_Window** window, SDL_Renderer** renderer);
void destroy_graphics(SDL_Window* window, SDL_Renderer* renderer);
void draw_graphics(unsigned char* gfx, SDL_Renderer* renderer);
