#include <iostream>
#include "entt/entt.hpp"
#include "sol2/sol.hpp"
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
int ballvel;
entt::registry registry;
entt::entity lpad, rpad, ball;
sol::state lua;

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
    ballvel = 8;
    lua.open_libraries(sol::lib::base, sol::lib::package);
    lua.script_file("src/scripts/EnemyAI.lua");
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
    /* Lua parameters */
    lua["parameters"] = lua.create_table_with(
        "padx", rpadrect.x,
        "pady", rpadrect.y,
        "padw", rpadrect.w,
        "padh", rpadrect.h,
        "velx", 0,
        "vely", PADVEL,
        "minx", 0,
        "maxx", WIDTH,
        "miny", 0,
        "maxy", HEIGHT,
        "ballw", ballrect.w,
        "ballh", ballrect.h
    );
    printf("Program set up correctly\n");
}

void Serve() {
    int num = rand() % 11;
    if(num <= 5) {
        registry.emplace<velocity>(ball,(float)-8.0,(float)8.0);
    } else {
        registry.emplace<velocity>(ball,(float)8.0,(float)8.0);
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
        }
        if(keystts[SDL_SCANCODE_S]) {
            auto pad = registry.get<velocity,figure>(lpad);
            std::get<1>(pad).bd.y += (int)std::get<0>(pad).dy;
        }
    }
}

void Update() {
    auto lpadrect = registry.get<velocity,figure>(lpad);
    auto rpadrect = registry.get<velocity,figure>(rpad);
    auto ballrect = registry.get<velocity,figure>(ball);
    /* left pad */
    if(std::get<1>(lpadrect).bd.y <= 0) { std::get<1>(lpadrect).bd.y = 0; }
    if(std::get<1>(lpadrect).bd.y+std::get<1>(lpadrect).bd.h>HEIGHT) { 
        std::get<1>(lpadrect).bd.y = HEIGHT - std::get<1>(lpadrect).bd.h; 
    }
    /* right pad */
    sol::protected_function func = lua["GetPosition"];
	auto result = func(std::get<1>(ballrect).bd.x,std::get<1>(ballrect).bd.y);
	if (result.valid()) {
        std::tuple<int,int> values = result;
		std::get<1>(rpadrect).bd.x = std::get<0>(values);
        std::get<1>(rpadrect).bd.y = std::get<1>(values);
	} else {
		sol::error err = result;
		std::string what = err.what();
		std::cout << what << std::endl;
	}
    /* ball */
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
    if(std::get<1>(ballrect).bd.y+std::get<1>(ballrect).bd.h >= HEIGHT) {
        std::get<0>(ballrect).dy = -std::get<0>(ballrect).dy;
    }
    if(SDL_HasIntersection(&std::get<1>(ballrect).bd,&std::get<1>(rpadrect).bd)) {
        double rel = (std::get<1>(rpadrect).bd.y+(std::get<1>(rpadrect).bd.h/2))-
                     (std::get<1>(ballrect).bd.y+(std::get<1>(ballrect).bd.w/2));
        double norm = rel / (std::get<1>(rpadrect).bd.h/2);
        double bounce = norm*(5*PI/2);
        ballvel += 1;
        std::get<0>(ballrect).dx = -ballvel * -cos(bounce);
        std::get<0>(ballrect).dy = ballvel * -sin(bounce);
    }
    if(SDL_HasIntersection(&std::get<1>(ballrect).bd,&std::get<1>(lpadrect).bd)) {
        double rel = (std::get<1>(lpadrect).bd.y+(std::get<1>(lpadrect).bd.h/2))-
                     (std::get<1>(ballrect).bd.y+(std::get<1>(ballrect).bd.w/2));
        double norm = rel / (std::get<1>(lpadrect).bd.h/2);
        double bounce = norm*(5*PI/2);
        ballvel += 1;
        std::get<0>(ballrect).dx = ballvel * cos(bounce);
        std::get<0>(ballrect).dy = ballvel * sin(bounce);
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