#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* plyr_tex = nullptr;

int main(int argc,char* argv[]) {
    /* SDL Init */
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error initializing SDL: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("PONG",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,720,720,0);
    if(window == NULL) {
        printf("Error generating window: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    renderer = SDL_CreateRenderer(window,1,SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Error initializing renderer: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    /* Image Init */
    /* Environment cleaning */
    printf("Cleaning up environment ...\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("Closing game ...\n");
    SDL_Quit();
    return EXIT_SUCCESS;
}