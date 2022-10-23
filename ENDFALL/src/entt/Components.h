#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "entt.hpp"

struct background {
    SDL_Texture* tex;
};

struct velocity {
    int dx;
    int dy;
};

struct stats {
    int health;
    int attack;
};

struct figure {
    SDL_Rect bd;
};

struct sprites {
    SDL_Texture* sprtsht;
};

struct door {
    int sceneid;
    SDL_Rect bd;
    int newPosX;
    int newPosY;
};

struct sceneflags {
    int id;
    bool visited; /* 0 = not visisted,1 = visited*/
    int enemies; /* Num enemies */
    int minX;
    int maxX;
    int minY;
    int maxY;
    std::vector<entt::entity> doors;
};


