#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* plyr_tex = nullptr;
int last_frm, frm_cnt, tmr_fps, fps;
int running;

void Update() {
}

void Render() {
    SDL_SetRenderDrawColor(renderer,71,107,107,255);
    SDL_RenderClear(renderer);
    frm_cnt++;
    tmr_fps = SDL_GetTicks()-last_frm;
    if(tmr_fps<(1000/60)) {
        SDL_Delay((1000/60)-tmr_fps);
    }
    SDL_RenderCopy(renderer,plyr_tex,NULL,NULL);
    SDL_RenderPresent(renderer);
}

void Input() {
    SDL_Event event;
    const Uint8* keystts = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT | keystts[SDL_SCANCODE_ESCAPE]) { running = 0; }
        if(keystts[SDL_SCANCODE_W]) { printf("Moving character up\n"); }
        if(keystts[SDL_SCANCODE_S]) { printf("Moving character down\n"); }
        if(keystts[SDL_SCANCODE_A]) { printf("Moving character left\n"); }
        if(keystts[SDL_SCANCODE_D]) { printf("Moving character right\n"); }
    }
}

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
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* surf = IMG_Load("MainC.png");
    if(surf == NULL) {
        printf("Image not loaded ...\n");
    }
    plyr_tex = SDL_CreateTextureFromSurface(renderer,surf);
    SDL_FreeSurface(surf);
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