#include <iostream>
#include "entt/entt.hpp"
#include <SDL2/SDL.h>
#include <tuple>
#include <math.h>
#include <time.h>

#define WIDTH 1080
#define HEIGHT 720
#define PADVEL 10
#define PI 3.141592

SDL_Window* window;
SDL_Renderer* renderer;
bool isRunning;
int frm_cnt, last_frm, lst_count, tmr_fps, fps;
entt::registry registry;
entt::entity lpad, rpad, ball;

struct velocity {
    float dx;
    float dy;
};

struct figure {
    SDL_Rect bd;
};

void Init() {
    printf("Setting up game...\n");
    /* left pad */
    lpad = registry.create();
    registry.emplace<velocity>(lpad,0.0f,(float)PADVEL);
    SDL_Rect lpadrect;
    lpadrect.w = 12;
    lpadrect.h = HEIGHT/4;
    lpadrect.x = 32;
    lpadrect.y = (HEIGHT/2)-((HEIGHT/4)/2);
    registry.emplace<figure>(lpad,lpadrect);
    /* right pad */
    rpad = registry.create();
    registry.emplace<velocity>(rpad,0.0f,(float)PADVEL);
    SDL_Rect rpadrect;
    rpadrect.w = 12;
    rpadrect.h = HEIGHT/4;
    rpadrect.x = WIDTH - 44;
    rpadrect.y = (HEIGHT/2)-((HEIGHT/4)/2);
    registry.emplace<figure>(rpad,rpadrect);
    /* ball */
    ball = registry.create();
    SDL_Rect ballrect;
    ballrect.w = 16;
    ballrect.h = 16;
    ballrect.x = (WIDTH / 2) - 8;
    ballrect.y = 20;
    registry.emplace<figure>(ball,ballrect);
    printf("Program set up correctly\n");
}

void Serve() {
    int num = rand() % 11;
    if(num <= 5) {
        registry.emplace<velocity>(ball,-8.0,8.0);
    } else {
        registry.emplace<velocity>(ball,8.0,8.0);
    }
}

void Render() {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    frm_cnt++;
    tmr_fps = SDL_GetTicks()-last_frm;
    if(tmr_fps<(1000/60)) {
        SDL_Delay((1000/60)-tmr_fps);
    }
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    auto view = registry.view<figure>();
    for(auto entity: view) {
        auto body = view.get<figure>(entity);
        SDL_RenderFillRect(renderer,&body.bd);
    }
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
            auto pad = registry.get<velocity,figure>(lpad);
            std::get<1>(pad).bd.y -= (int)std::get<0>(pad).dy;
            printf("%d\n",std::get<1>(pad).bd.y);
        }
        if(keystts[SDL_SCANCODE_S]) {
            auto pad = registry.get<velocity,figure>(lpad);
            std::get<1>(pad).bd.y += (int)std::get<0>(pad).dy;
        }
        if(keystts[SDL_SCANCODE_UP]) {
            auto pad = registry.get<velocity,figure>(rpad);
            std::get<1>(pad).bd.y -= (int)std::get<0>(pad).dy;
        }
        if(keystts[SDL_SCANCODE_DOWN]) {
            auto pad = registry.get<velocity,figure>(rpad);
            std::get<1>(pad).bd.y += (int)std::get<0>(pad).dy;
        }
    }
}

void Update() {
    /* left pad */
    auto lpadrect = registry.get<velocity,figure>(lpad);
    if(std::get<1>(lpadrect).bd.y <= 0) { std::get<1>(lpadrect).bd.y = 0; }
    if(std::get<1>(lpadrect).bd.y+std::get<1>(lpadrect).bd.h>HEIGHT) { 
        std::get<1>(lpadrect).bd.y = HEIGHT - std::get<1>(lpadrect).bd.h; 
    }
    /* right pad */
    auto rpadrect = registry.get<velocity,figure>(rpad);
    if(std::get<1>(rpadrect).bd.y < 0) { std::get<1>(rpadrect).bd.y = 0; }
    if(std::get<1>(rpadrect).bd.y+std::get<1>(rpadrect).bd.h>HEIGHT) { 
        std::get<1>(rpadrect).bd.y = HEIGHT - std::get<1>(rpadrect).bd.h; 
    }
    /* ball */
    auto ballrect = registry.get<velocity,figure>(ball);
    if(std::get<1>(ballrect).bd.x <= 0) { 
        printf("Player 2 won!\n");
        isRunning = false;
    }
    if(std::get<1>(ballrect).bd.x+std::get<1>(ballrect).bd.w >= WIDTH) {
        printf("Player 1 won!\n");
        isRunning = false;
    }
    if(std::get<1>(ballrect).bd.y <= 0) { 
        std::get<0>(ballrect).dy = -std::get<0>(ballrect).dy;
    }
    if(std::get<1>(ballrect).bd.y+std::get<1>(ballrect).bd.w >= WIDTH) {
        std::get<0>(ballrect).dy = -std::get<0>(ballrect).dy;
    }
    if(SDL_HasIntersection(&std::get<1>(ballrect).bd,&std::get<1>(rpadrect).bd)) {
        double rel = (std::get<1>(rpadrect).bd.y+(std::get<1>(rpadrect).bd.h/2))-
                     (std::get<1>(ballrect).bd.y+(std::get<1>(ballrect).bd.w/2));
        double norm = rel / (std::get<1>(rpadrect).bd.h/2);
        double bounce = norm*(5*PI/2);
        std::get<0>(ballrect).dx = 8.0 * cos(bounce);
        std::get<0>(ballrect).dy = 8.0 * -sin(bounce);
    }
    if(SDL_HasIntersection(&std::get<1>(ballrect).bd,&std::get<1>(lpadrect).bd)) {
        double rel = (std::get<1>(lpadrect).bd.y+(std::get<1>(lpadrect).bd.h/2))-
                     (std::get<1>(ballrect).bd.y+(std::get<1>(ballrect).bd.w/2));
        double norm = rel / (std::get<1>(lpadrect).bd.h/2);
        double bounce = norm*(5*PI/2);
        std::get<0>(ballrect).dx = 8.0 * cos(bounce);
        std::get<0>(ballrect).dy = 8.0 * -sin(bounce);
    }
    std::get<1>(ballrect).bd.x += std::get<0>(ballrect).dx;
    std::get<1>(ballrect).bd.y += std::get<0>(ballrect).dy;
}

int main(int argc,char* argv[]) {
    srand(time(NULL));
    isRunning = true;
    static int lst_time = 0;
    printf("Initializing program...\n");
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error initializing SDL: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("Entt Pong",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,0);
    if(window == NULL) {
        printf("Error creating window: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Error creating renderer: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    printf("Program initialized correctly\n");
    Init();
    Serve();
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