#include <iostream>
#include "entt/entt.hpp"
#include "entt/Components.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <tuple>
#include <time.h>

#define WIDTH 800
#define HEIGHT 640

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* bckgr;
SDL_Texture* plyr;
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
        void LoadScenes() {
            SDL_Texture* bkgr = nullptr;
            /* Scene 1 */
            entt::entity scene1 = registry.create();
            bkgr = IMG_LoadTexture(renderer,"./res/MainEntrance.png");
            registry.emplace<background>(scene1,bkgr);
            entt::entity door1A = registry.create();
            SDL_Rect door1Arect;
            door1Arect.w = 23;
            door1Arect.h = 32;
            door1Arect.x = 708;
            door1Arect.y = 317;
            registry.emplace<door>(door1A,1,door1Arect,708,354);
            std::vector<entt::entity> doors1;
            doors1.push_back(door1A);
            registry.emplace<sceneflags>(scene1,0,false,0, \
                                        0,WIDTH,312,HEIGHT,doors1);
            scenes.push_back(scene1);
            /* Scene 2 */
            entt::entity scene2 = registry.create();
            bkgr = IMG_LoadTexture(renderer,"./res/Area1.png");
            registry.emplace<background>(scene2,bkgr);
            entt::entity door2A = registry.create();
            SDL_Rect door2Arect;
            door2Arect.w = 23;
            door2Arect.h = 32;
            door2Arect.x = 708;
            door2Arect.y = 317;
            registry.emplace<door>(door2A,0,door2Arect,708,354);
            entt::entity door2B = registry.create();
            SDL_Rect door2Brect;
            door2Brect.w = 23;
            door2Brect.h = 32;
            door2Brect.x = 129;
            door2Brect.y = 317;
            registry.emplace<door>(door2B,2,door2Brect,389,80);
            std::vector<entt::entity> doors2;
            doors2.push_back(door2A);
            doors2.push_back(door2B);
            registry.emplace<sceneflags>(scene2,1,false,2, \
                                        0,WIDTH,312,HEIGHT,doors2);
            scenes.push_back(scene2);
            /* Scene 3 */
            entt::entity scene3 = registry.create();
            bkgr = IMG_LoadTexture(renderer,"./res/Hall1.png");
            registry.emplace<background>(scene3,bkgr);
            entt::entity door3A = registry.create();
            SDL_Rect door3Arect;
            door3Arect.w = 23;
            door3Arect.h = 32;
            door3Arect.x = 389;
            door3Arect.y = 43;
            registry.emplace<door>(door3A,1,door3Arect,129,354);
            entt::entity door3B = registry.create();
            SDL_Rect door3Brect;
            door3Brect.w = 23;
            door3Brect.h = 32;
            door3Brect.x = 389;
            door3Brect.y = 560;
            registry.emplace<door>(door3B,3,door3Brect,90,354);
            std::vector<entt::entity> doors3;
            doors3.push_back(door3A);
            doors3.push_back(door3B);
            registry.emplace<sceneflags>(scene3,2,false,0, \
                                        301,499,45,564,doors3);
            scenes.push_back(scene3);
            /* Scene 4 */
            entt::entity scene4 = registry.create();
            bkgr = IMG_LoadTexture(renderer,"./res/Area2.png");
            registry.emplace<background>(scene4,bkgr);
            entt::entity door4A = registry.create();
            SDL_Rect door4Arect;
            door4Arect.w = 23;
            door4Arect.h = 32;
            door4Arect.x = 90;
            door4Arect.y = 317;
            registry.emplace<door>(door4A,2,door4Arect,389,485);
            entt::entity door4B = registry.create();
            SDL_Rect door4Brect;
            door4Brect.w = 23;
            door4Brect.h = 32;
            door4Brect.x = 340;
            door4Brect.y = 317;
            registry.emplace<door>(door4B,4,door4Brect,280,415);
            std::vector<entt::entity> doors4;
            doors4.push_back(door4A);
            doors4.push_back(door4B);
            registry.emplace<sceneflags>(scene4,3,false,3, \
                                        0,WIDTH,312,HEIGHT,doors4);
            scenes.push_back(scene4);
            /* Scene 5 */
            entt::entity scene5 = registry.create();
            bkgr = IMG_LoadTexture(renderer,"./res/Area3.png");
            registry.emplace<background>(scene5,bkgr);
            entt::entity door5A = registry.create();
            SDL_Rect door5Arect;
            door5Arect.w = 23;
            door5Arect.h = 32;
            door5Arect.x = 280;
            door5Arect.y = 484;
            registry.emplace<door>(door5A,3,door5Arect,340,354);
            std::vector<entt::entity> doors5;
            doors5.push_back(door5A);
            registry.emplace<sceneflags>(scene5,4,false,0, \
                                        224,576,120,491,doors5);
            scenes.push_back(scene5);
        }
        void SetBkgrID(uint8_t num) {
            this->id = num;
        }
        SDL_Texture* GetBckgrImage() {
            auto scene = registry.get<background>(this->scenes.at(this->id));
            return scene.tex;
        }
        int* GetLimits() {
            auto scene = registry.get<sceneflags>(this->scenes.at(this->id)); 
            int* limits = new int[4];
            limits[0] = scene.minX;
            limits[1] = scene.minY;
            limits[2] = scene.maxX;
            limits[3] = scene.maxY;
            return limits;            
        }
        std::vector<entt::entity> GetDoors() {
            auto scene = registry.get<sceneflags>(this->scenes.at(this->id));  
            return scene.doors;
        }
        void SetVisitedRoom() {
            auto scene = registry.get<sceneflags>(this->scenes.at(this->id));
            scene.visited = true;
        }
        int GetNumEnemies() {
            auto scene = registry.get<sceneflags>(this->scenes.at(this->id));
            return scene.enemies;
        }
        void UpdateNumEnemies() {
            auto scene = registry.get<sceneflags>(this->scenes.at(this->id));
            scene.enemies = scene.enemies - 1;
        }
        ~EnvironmentSystem() {
            SDL_FreeSurface(this->currBkgr);
            this->scenes.clear();
        }
};

class MCSystem {
    private:
        entt::entity player;
        SDL_Rect sprtrect;
        bool front; /* sprite direction */
        bool atacking;
        bool first; /* sprite loop */
        int animtime; /* Animation duration */
        time_t atkcounter;
    public:
        MCSystem() {
            this->player = registry.create();
            registry.emplace<velocity>(this->player,6,6);
            registry.emplace<stats>(this->player,100,50);
            SDL_Rect rect;
            rect.w = rect.h = 64;
            rect.x = 60;
            rect.y = 460;
            this->front = true;
            registry.emplace<figure>(this->player,rect);
            this->sprtrect.w = this->sprtrect.h = 32;
            this->sprtrect.y = 32;
            this->sprtrect.x = 0;
            this->first = true;
            this->animtime = 1;
        }
        void LoadCharacter() {
            SDL_Texture* plyrsprt = IMG_LoadTexture(renderer,"./res/MainC.png");
            registry.emplace<sprites>(this->player,plyrsprt);
        }
        SDL_Texture* GetPlyrSprite() {
            auto plyr = registry.get<sprites>(this->player);
            return plyr.sprtsht;
        }
        SDL_Rect GetPlyrRect() {
            auto plyr = registry.get<figure>(this->player);
            return plyr.bd;
        }
        SDL_Rect GetSprtRect() {
            if(this->atacking) {
                if(time(0)-this->atkcounter == this->animtime) {
                    this->atacking = false;
                }
            }
            if(this->front) {
                this->sprtrect.y = 32;
                this->sprtrect.x = 0;
            } 
            if(!this->front) {
                this->sprtrect.y = 0;
                this->sprtrect.x = 0;
            }
            if(this->front and this->atacking) {
                this->sprtrect.y = 64;
                this->sprtrect.x = 32;
            } 
            if(!this->front and this->atacking) {
                this->sprtrect.y = 96;
                this->sprtrect.x = 32;
            }
            return this->sprtrect;
        }
        void SetPosition(int x,int y) {
            auto plyr = registry.get<figure,velocity>(this->player);
            std::get<0>(plyr).bd.x = x;
            std::get<0>(plyr).bd.y = y;
        }
        void Move(int way) {
            auto plyr = registry.get<velocity,figure>(this->player);
            if(way == 1) {
                std::get<1>(plyr).bd.y -= (int)std::get<0>(plyr).dy;
            }
            if(way == 2) {
                std::get<1>(plyr).bd.y += (int)std::get<0>(plyr).dy;
            }
            if(way == 3) {
                std::get<1>(plyr).bd.x += (int)std::get<0>(plyr).dx;
            }
            if(way == 4) {
                std::get<1>(plyr).bd.x -= (int)std::get<0>(plyr).dx;
            }
        }
        void SetDirection(bool direc) {
            this->front = direc;
        }
        void ActivateAttack() {
            this->atacking = true;
            this->atkcounter = time(0);
        }
        bool IsAttacking() {
            return this->atacking;
        }
        ~MCSystem() {
            
        }
};

class EnemySystem {
    private:
        entt::entity enemy;
        SDL_Rect sprtrect;
        bool front;
    public:
        EnemySystem(int x,int y,bool front) {
            this->front = front;
            this->enemy = registry.create();
            registry.emplace<velocity>(this->enemy,4,4);
            registry.emplace<stats>(this->enemy,100,20);
            SDL_Rect rect;
            rect.w = rect.h = 64;
            rect.x = x;
            rect.y = y;
            registry.emplace<figure>(this->enemy,rect);
            this->sprtrect.w = this->sprtrect.h = 32;
            this->sprtrect.x = 0;
            if(this->front) {
                this->sprtrect.y = 32;
            } else {
                this->sprtrect.y = 0;
            }
        }
        void LoadCharacter() {
            SDL_Texture* enmysprt = IMG_LoadTexture(renderer,"./res/Villain1.png");
            registry.emplace<sprites>(this->enemy,enmysprt);
        }
        SDL_Texture* GetEnemySprite() {
            auto enemy = registry.get<sprites>(this->enemy);
            return enemy.sprtsht;
        }
        SDL_Rect GetEnemyRect() {
            auto enemy = registry.get<figure>(this->enemy);
            return enemy.bd;
        }
        SDL_Rect GetSprtRect() {
            if(this->front) {
                this->sprtrect.y = 32;
            } 
            if(!this->front) {
                this->sprtrect.y = 0;
            }
            return this->sprtrect;
        }
        ~EnemySystem() {}
};

EnvironmentSystem envSystem = EnvironmentSystem();
MCSystem mcSystem = MCSystem();
std::vector<EnemySystem> enemies;

void Init() {
    printf("Setting up game...\n");
    srand(time(NULL));
    envSystem.LoadScenes();
    mcSystem.LoadCharacter();
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
    plyr = mcSystem.GetPlyrSprite();
    SDL_Rect plyrrect = mcSystem.GetPlyrRect();
    SDL_Rect sprtrect = mcSystem.GetSprtRect();
    SDL_RenderCopy(renderer,plyr,&sprtrect,&plyrrect);
    if(enemies.size() > 0) {
        for(EnemySystem enemy: enemies) {
            SDL_Texture* enemytex = enemy.GetEnemySprite();
            SDL_Rect enmyrect = enemy.GetEnemyRect();
            SDL_Rect enmysprt = enemy.GetSprtRect();
            SDL_RenderCopy(renderer,enemytex,&enmysprt,&enmyrect);
        }
    }
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
            mcSystem.Move(1);
        }
        if(keystts[SDL_SCANCODE_S]) {
            mcSystem.Move(2);
        }
        if(keystts[SDL_SCANCODE_D]) { 
            mcSystem.SetDirection(true);
            mcSystem.Move(3);
        }
        if(keystts[SDL_SCANCODE_A]) { 
            mcSystem.SetDirection(false);
            mcSystem.Move(4);
        }
        if(keystts[SDL_SCANCODE_SPACE]) {
            mcSystem.ActivateAttack();
        }
   }
};

void Update() {
    int* bounds = envSystem.GetLimits();
    SDL_Rect plyrrect = mcSystem.GetPlyrRect();
    if(plyrrect.x < bounds[0]-10) {
        mcSystem.SetPosition(bounds[0]-10,plyrrect.y);
    }
    if(plyrrect.y < bounds[1]-10) {
        mcSystem.SetPosition(plyrrect.x,bounds[1]-10);
    }
    if(plyrrect.x + plyrrect.w > bounds[2]+10) {
        mcSystem.SetPosition(bounds[2]-plyrrect.w+10,plyrrect.y);
    }
    if(plyrrect.y + plyrrect.h > bounds[3]) {
        mcSystem.SetPosition(plyrrect.x,bounds[3]-plyrrect.h);
    }
    std::vector<entt::entity> scndoors = envSystem.GetDoors();
    for(entt::entity scndoor: scndoors) {
        auto comp = registry.get<door>(scndoor);
        if(SDL_HasIntersection(&plyrrect,&comp.bd)) {
            enemies.clear();
            mcSystem.SetPosition(comp.newPosX,comp.newPosY);
            envSystem.SetBkgrID(comp.sceneid);
            envSystem.SetVisitedRoom();
            int numenemies = envSystem.GetNumEnemies();
            if(numenemies > 0) {
                for(int x = 0;x < numenemies;x++) {
                    int posx = (rand() % (bounds[2] - bounds[0] - 64)) + bounds[0];
                    int posy = (rand() % (bounds[3] - bounds[1] - 64)) + bounds[1];
                    int direction = rand() % 2;
                    EnemySystem enemy = EnemySystem(posx,posy,(bool)direction);
                    enemy.LoadCharacter();
                    enemies.push_back(enemy);
                }
            }
        }
    }
    printf("%d\n",mcSystem.IsAttacking());
};

void CleanEnvironment() {
    printf("Cleaning up environment...\n");
    envSystem.~EnvironmentSystem();
    mcSystem.~MCSystem();
    registry.clear();
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
        Update();
        Render();
    }
    CleanEnvironment();
    return EXIT_FAILURE;
};