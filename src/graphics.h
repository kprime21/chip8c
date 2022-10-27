#include <SDL2/SDL.h>

//double pointers because we are changing what they point to
void start_graphics(SDL_Window** window, SDL_Renderer** renderer,SDL_Texture** texture);
void destroy_graphics(SDL_Window* window, SDL_Renderer* renderer);
void draw_graphics(unsigned char* gfx, SDL_Renderer* renderer);
