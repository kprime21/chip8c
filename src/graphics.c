#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "cpu.h"

//graphics here

int main(){
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    int posX = 100, posY = 100, width = 320, height = 240;
    SDL_bool loopShouldStop = SDL_FALSE;
    int fps = 120;
    int desiredDelta = 1000/fps;
    chip8 test;
    printf("%d\n", font_reg[0]);
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Chip8 Emulator", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);


    while (!loopShouldStop)
    {
        //start time
        int startTime = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    loopShouldStop = SDL_TRUE;
                    break;
            }
        }
    for(int y = 0; y < 240; y++){

        for(int x = 0; x < 320; x++){
            int randoma = (rand() % 255) +1;
            int randomb = (rand() % 255) +1;
            int randomc = (rand() % 255) +1;
            
            SDL_SetRenderDrawColor(renderer, randoma, randomb, randomc, 255);
            SDL_RenderDrawPoint(renderer,x,y);

            //set FPS to 60
            int delta = SDL_GetTicks() - startTime;
            if(delta < desiredDelta){
                SDL_Delay(desiredDelta - delta);
            }
            
          
         
        }
    }
    SDL_RenderPresent(renderer);
    }

   
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();
    

    return 0;
}