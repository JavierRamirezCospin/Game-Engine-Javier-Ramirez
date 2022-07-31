#include <iostream>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Color color;
SDL_Rect l_pad, r_pad, ball;
double pi = 3.14159265;
bool running = true;
int frm_cnt, tmr_fps, last_frm, fps, b_size, b_speed, p_speed;
float velX, velY;
int w_width, w_height;

void Serve() {
    ball.x = (w_width / 2) - (ball.w / 2);
    ball.y = 20;
    velY = b_speed/2;
    int num = rand() % 11;
    num <= 5 ? velX = -b_speed/2 : velX = b_speed/2;
}

void Update() {
    if(l_pad.y<0) { l_pad.y = 0; }
    if(l_pad.y+l_pad.h>w_height) { l_pad.y = w_height-l_pad.h; }
    if(r_pad.y<0) { r_pad.y = 0; }
    if(r_pad.y+r_pad.h>w_height) { r_pad.y = w_height-r_pad.h; }
    if(ball.x <= 0) { 
        printf("Player 1 won ...\n"); 
        running = false;
    }
    if(ball.x + ball.w >= w_width) { 
        ("Player 2 won ...\n"); 
        running = false;    
    }
    if(ball.y <= 0) { velY = -velY; }
    if(ball.y+b_size >= w_height) { velY = -velY; }
    if(SDL_HasIntersection(&ball,&r_pad)) { 
        double rel = (r_pad.y + (r_pad.h/2))-(ball.y+(b_size/2));
        double norm = rel/(r_pad.h/2);
        double bounce = norm*(5*pi/2);
        velX = b_speed * cos(bounce);
        velY = b_speed * -sin(bounce);
    }
    if(SDL_HasIntersection(&ball,&l_pad)) { 
        double rel = (l_pad.y + (l_pad.h/2))-(ball.y+(b_size/2));
        double norm = rel/(l_pad.h/2);
        double bounce = norm*(5*pi/2);
        velX = b_speed * cos(bounce);
        velY = b_speed * -sin(bounce);
    }
    ball.x += velX;
    ball.y += velY;
}

void Input() {
    SDL_Event event;
    const Uint8* keystts = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT | keystts[SDL_SCANCODE_ESCAPE]) { 
            printf("Closing game ...\n");
            running = false; 
        }
        if(keystts[SDL_SCANCODE_DOWN]) { r_pad.y += p_speed; }
        if(keystts[SDL_SCANCODE_UP]) { r_pad.y -= p_speed; }
        if(keystts[SDL_SCANCODE_W]) { l_pad.y -= p_speed; }
        if(keystts[SDL_SCANCODE_S]) { l_pad.y += p_speed; }
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
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,255);
    SDL_RenderFillRect(renderer,&l_pad);
    SDL_RenderFillRect(renderer,&r_pad);
    SDL_RenderFillRect(renderer,&ball);
    SDL_RenderPresent(renderer);
}

int main(int argc,char* argv[]) {
    /* Init */
    srand(time(NULL));
    w_width = 1080, w_height = 720, b_size = 16, b_speed = 16, p_speed = 11;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error initializing SDL: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    printf("Initialized correctly ...\n");
    window = SDL_CreateWindow("PONG",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w_width,w_height,0);
    if(window == NULL) {
        printf("Error generating window: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    printf("Window generated successfully\n");
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Error generating renderer: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    printf("Renderer generated successfully\n");
    color.r = color.g = color.b = 255;
    /* Left paddle */
    l_pad.x = 32;
    l_pad.w = 12;
    l_pad.h = w_height / 4;
    l_pad.y = (w_height/2)-(l_pad.h/2);
    /* Right paddle */
    r_pad = l_pad;
    r_pad.x = w_width - r_pad.w - 32;
    /* Ball */
    ball.w = ball.h = b_size;
    Serve();
    static int lst_time = 0;
    while(running) {
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
    printf("Cleaning up environment ...\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Environment cleaning successful ...\n");
    return EXIT_SUCCESS;
}