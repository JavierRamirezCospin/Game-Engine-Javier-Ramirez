#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct background {
    SDL_Texture* tex;
};

struct velocity {
    uint8_t dx;
    uint8_t dy;
};

struct stats {
    uint8_t health;
    uint8_t attack;
};

struct figure {
    SDL_Rect bd;
};

struct sprites {
    SDL_Texture* idle_l;
    SDL_Texture* idle_r;
    SDL_Texture* atck_l;
    SDL_Texture* atck_r;
    std::vector<SDL_Texture*> walk_l;
    std::vector<SDL_Texture*> walk_r;
};

struct door {
    uint8_t sceneid;
    SDL_Rect bd;
};

struct sceneflags {
    uint8_t id;
    bool visited; /* 0 = not visisted,1 = visited*/
    uint8_t enemies; /* Num enemies */
    uint16_t minX;
    uint16_t maxX;
    uint16_t minY;
    uint16_t maxY;
    //std::vector<door> doors;
};


