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
SDL_Texture* bckgr;
bool isRunning;
int frm_cnt, last_frm, lst_count, tmr_fps, fps;
entt::registry registry;

class EnvironmentSystem {
    private:
        uint8_t id;
        SDL_Surface* currBkgr;
        std::vector<entt::entity> scenes;
    public:
        EnvironmentSystem() {
            this->id = 0;
        }
        void LoadScenes(SDL_Renderer* renderer) {
            SDL_Texture* bkgr = nullptr;
            /* Scene 1 */
            entt::entity scene1 = registry.create();
            bkgr = IMG_LoadTexture(renderer,"./res/MainEntrance.png");
            registry.emplace<background>(scene1,bkgr);
            registry.emplace<sceneflags>(scene1,(uint8_t)0,true,(uint8_t)0,(uint8_t)0, \
                                        (uint8_t)WIDTH,(uint8_t)312,(uint8_t)HEIGHT);
            scenes.push_back(scene1);
            /* Scene 2 */
            entt::entity scene2 = registry.create();
            bkgr = IMG_LoadTexture(renderer,"./res/Area1.png");
            registry.emplace<background>(scene2,bkgr);
            registry.emplace<sceneflags>(scene2,(uint8_t)1,false,(uint8_t)1,(uint8_t)0, \
                                        (uint8_t)WIDTH,(uint8_t)312,(uint8_t)HEIGHT);
            scenes.push_back(scene2);
        }
        void SetBkgrID(uint8_t num) {
            this->id = num;
        }
        SDL_Texture* GetBckgrImage() {
            auto scene = registry.get<background>(this->scenes.at(this->id));
            return scene.tex;
        }
        ~EnvironmentSystem() {
            SDL_FreeSurface(this->currBkgr);
            this->scenes.clear();
        }
};

EnvironmentSystem envSystem = EnvironmentSystem();

void Init() {
    printf("Setting up game...\n");
    envSystem.LoadScenes(renderer);
    printf("Program set up correctly\n");
};

void Render() {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    frm_cnt++;
    tmr_fps = SDL_GetTicks()-last_frm;
    if(tmr_fps<(1000/60)) {
        SDL_Delay((1000/60)-tmr_fps);
    }
    bckgr = envSystem.GetBckgrImage();
    SDL_RenderCopy(renderer,bckgr,NULL,NULL);
    SDL_RenderPresent(renderer);
};

void Input() {
    SDL_Event event;
    const Uint8* keystts = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            isRunning = false;
        }
        if(keystts[SDL_SCANCODE_W]) {
            envSystem.SetBkgrID(1);
        }
        if(keystts[SDL_SCANCODE_S]) {
            envSystem.SetBkgrID(0);
        }
        if(keystts[SDL_SCANCODE_D]) { 

        }
        if(keystts[SDL_SCANCODE_A]) { 

        }
   }
};

void Update() {
    
};

void CleanEnvironment() {
    printf("Cleaning up environment...\n");
    envSystem.~EnvironmentSystem();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Environment cleaning successful\n");
};

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
    CleanEnvironment();
    return EXIT_FAILURE;
};