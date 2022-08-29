#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct backgroundimage {
    SDL_Texture* tex;
};

struct velocity {
    int dx;
    int dy;
};

struct figure {
    SDL_Rect bd;
};