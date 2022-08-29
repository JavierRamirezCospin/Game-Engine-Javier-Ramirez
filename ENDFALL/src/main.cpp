#include <iostream>
#include "entt/entt.hpp"
#include "entt/Components.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <tuple>

#define WIDTH 800
#define HEIGHT 640

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* currBackground;
bool isRunning;
int frm_cnt, last_frm, lst_count, tmr_fps, fps;
entt::registry registry;
entt::entity scene1, scene2;
entt::entity player;
int bckgrId;

void Init() {
    printf("Setting up game...\n");
    /* Scenes */
    SDL_Texture* bckgr1 = IMG_LoadTexture(renderer,"./res/MainEntrance.png");
    scene1 = registry.create();
    registry.emplace<backgroundimage>(scene1,bckgr1);
    SDL_Rect bckgr1rect;
    bckgr1rect.w = 23;
    bckgr1rect.h = 32;
    bckgr1rect.x = 708;
    bckgr1rect.y = 317;
    registry.emplace<figure>(scene1,bckgr1rect);
    SDL_Texture* bckgr2 = IMG_LoadTexture(renderer,"./res/Area1.png");
    scene2 = registry.create();
    registry.emplace<backgroundimage>(scene2,bckgr2);
    /* Player */
    SDL_Texture* plyr = IMG_LoadTexture(renderer,"./res/MainC.png");
    player = registry.create();
    registry.emplace<backgroundimage>(player,plyr);
    registry.emplace<velocity>(player,6,6);
    SDL_Rect plyrrect;
    plyrrect.w = plyrrect.h = 64;
    plyrrect.x = 60;
    plyrrect.y = 460;
    registry.emplace<figure>(player,plyrrect);
    printf("Program set up correctly\n");
}

void Render() {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    frm_cnt++;
    tmr_fps = SDL_GetTicks()-last_frm;
    if(tmr_fps<(1000/60)) {
        SDL_Delay((1000/60)-tmr_fps);
    }
    switch(bckgrId) {
        case 2: {
            auto img = registry.get<backgroundimage>(scene2);
            currBackground = img.tex;
            break;
        }
        default: {
            auto img = registry.get<backgroundimage>(scene1);
            currBackground = img.tex;
            break;
        }
    }
    SDL_RenderCopy(renderer,currBackground,NULL,NULL);
    auto plyr = registry.get<backgroundimage,figure>(player);
    SDL_RenderCopy(renderer,std::get<0>(plyr).tex,NULL,&std::get<1>(plyr).bd);
    SDL_RenderPresent(renderer);
}

void Input() {
    SDL_Event event;
    const Uint8* keystts = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            isRunning = false;
        }
        if(keystts[SDL_SCANCODE_W]) {
            auto plyr = registry.get<velocity,figure>(player);
            std::get<1>(plyr).bd.y -= (int)std::get<0>(plyr).dy;
        }
        if(keystts[SDL_SCANCODE_S]) {
            auto plyr = registry.get<velocity,figure>(player);
            std::get<1>(plyr).bd.y += (int)std::get<0>(plyr).dy;
         }
        if(keystts[SDL_SCANCODE_A]) { 
            auto plyr = registry.get<velocity,figure>(player);
            std::get<1>(plyr).bd.x -= (int)std::get<0>(plyr).dx;
        }
        if(keystts[SDL_SCANCODE_D]) { 
            auto plyr = registry.get<velocity,figure>(player);
            std::get<1>(plyr).bd.x += (int)std::get<0>(plyr).dx;
        }
    }
}

void Update() {
    auto plyr = registry.get<velocity,figure>(player);
    auto scene = registry.get<backgroundimage,figure>(scene1);
    if(std::get<1>(plyr).bd.x <= 0) {
        std::get<1>(plyr).bd.x = 0;
    }
    if(std::get<1>(plyr).bd.x + std::get<1>(plyr).bd.w > WIDTH) {
        std::get<1>(plyr).bd.x = WIDTH - std::get<1>(plyr).bd.w;
    }
    if(std::get<1>(plyr).bd.y + std::get<1>(plyr).bd.h > HEIGHT) {
        std::get<1>(plyr).bd.y = HEIGHT - std::get<1>(plyr).bd.h;
    }
    if(std::get<1>(plyr).bd.y <= 312) {
        std::get<1>(plyr).bd.y = 312;
    }
    if(SDL_HasIntersection(&std::get<1>(plyr).bd,&std::get<1>(scene).bd)) {
        bckgrId = 2;
    }
}

int main(int argc,char* argv[]) {
    isRunning = true;
    static int lst_time = 0;
    printf("Initializing program...\n");
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error initializing SDL: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("ENDFALL",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,0);
    if(window == NULL) {
        printf("Error creating window: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Error creating renderer: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    int img_init = IMG_Init(IMG_INIT_PNG);
    if(img_init <= 0) {
        printf("IMG failed to initialize: %s...\n",IMG_GetError());
        return EXIT_FAILURE;
    }
    printf("Program initialized correctly\n");
    Init();
    while(isRunning) {
        last_frm = SDL_GetTicks();
        if(last_frm >= (lst_time+1000)) {
            lst_time = last_frm;
            fps = frm_cnt;
            frm_cnt = 0;
            printf("FPS: %d\n",fps);
        }
        Input();
        Render();
        Update();
    }
    printf("Cleaning up environment...\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Environment cleaning successful\n");
    return EXIT_FAILURE;
}