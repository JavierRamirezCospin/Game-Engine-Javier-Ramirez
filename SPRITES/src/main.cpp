#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 640

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* bkgr_tex = nullptr;
SDL_Texture* plyr_tex = nullptr;
SDL_Rect srcRect, destRect;

int last_frm, frm_cnt, tmr_fps, fps;
int speed = 2;
int running;

void Update() {
    if(destRect.x < 0) { destRect.x = 0; }
    if(destRect.x > WIDTH-destRect.w) { destRect.x = WIDTH-destRect.w; }
    if(destRect.y < (HEIGHT/2)-(destRect.h/2)) { destRect.y = (HEIGHT/2)-(destRect.h/2); }
    if(destRect.y > HEIGHT-destRect.h) { destRect.y = HEIGHT-destRect.h; }
}

void Render() {
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    frm_cnt++;
    tmr_fps = SDL_GetTicks()-last_frm;
    if(tmr_fps<(1000/60)) {
        SDL_Delay((1000/60)-tmr_fps);
    }
    SDL_RenderCopy(renderer,bkgr_tex,NULL,NULL);
    SDL_RenderCopy(renderer,plyr_tex,NULL,&destRect);
    SDL_RenderPresent(renderer);
}

void Input() {
    SDL_Event event;
    const Uint8* keystts = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT | keystts[SDL_SCANCODE_ESCAPE]) { running = 0; }
        if(keystts[SDL_SCANCODE_W]) { 
            destRect.y -= speed; 
        }
        if(keystts[SDL_SCANCODE_S]) { 
            destRect.y += speed;
        }
        if(keystts[SDL_SCANCODE_A]) { 
            destRect.x -= speed;
        }
        if(keystts[SDL_SCANCODE_D]) { 
            destRect.x += speed;    
        }
        if(keystts[SDL_SCANCODE_LSHIFT]) {
            speed = 5;
        }
    }
}

int main(int argc,char* argv[]) {
    /* SDL Init */
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error initializing SDL: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("PONG",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WIDTH,HEIGHT,0);
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
    int img_init = IMG_Init(IMG_INIT_PNG);
    if(img_init <= 0) {
        printf("IMG failed to initialize: %s...\n",IMG_GetError());
        return EXIT_FAILURE;
    }
    bkgr_tex = IMG_LoadTexture(renderer,"./res/MainEntrance.png");
    if(bkgr_tex == NULL) {
        printf("Image not loaded: %s\n",IMG_GetError());
    }
    plyr_tex = IMG_LoadTexture(renderer,"./res/MainC.png");
    if(plyr_tex == NULL) {
        printf("Player image not loaded: %s\n",IMG_GetError());
    }
    destRect.x = WIDTH/2;
    destRect.y = HEIGHT/2 + 32;
    destRect.h = destRect.w = 64;
    /* Main loop */
    running = 1;
    static int lst_time = 0;
    while(running) {
        last_frm = SDL_GetTicks();
        if(last_frm >= (lst_time+1000)) {
            lst_time = last_frm;
            fps = frm_cnt;
            frm_cnt = 0;
            printf("FPS: %d\n",fps);
        }
        Update();
        Input();
        Render();
    }
    /* Environment cleaning */
    printf("Cleaning up environment ...\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("Closing game ...\n");
    SDL_Quit();
    return EXIT_SUCCESS;
}