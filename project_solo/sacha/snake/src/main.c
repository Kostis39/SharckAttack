#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <string.h>
#include <time.h>

typedef struct{
    float x;
    float y;
} Vector;

void end_sdl(
    char ok,                // fin normale : ok = 0 ; anormale ok = 1
    char const* msg,        // message à afficher
    SDL_Window* window,     // fenêtre à fermer
    SDL_Renderer* renderer  // renderer à fermer
) {
    char msg_formated[255];
    int l;

    if (!ok) {  // Affichage de ce qui ne va pas
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");
        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL) {  // Destruction si nécessaire du renderer
        SDL_DestroyRenderer(renderer);  // Attention : on suppose que les NULL sont maintenus !!
        renderer = NULL;
    }

    if (window != NULL) {  // Destruction si nécessaire de la fenêtre
        SDL_DestroyWindow(window);  // Attention : on suppose que les NULL sont maintenus !!
        window = NULL;
    }

    SDL_Quit();

    if (!ok) {  // On quitte si cela ne va pas
        exit(EXIT_FAILURE);
    }
}

void draw(SDL_Window * window, SDL_Renderer * renderer, SDL_Rect * rect, Vector * dir, int window_w, int window_h){
    if (renderer == NULL || window == NULL){
        return;
    }
    SDL_SetRenderDrawColor(
        renderer,
        250, 0, 0,
        255
    );

    
    int new_x = rect->x + dir->x;
    int new_y = rect->y + dir->y;
    
    if (new_x <= 0) {
        new_x = 0;
        dir->x = rand() % 11 - 5;
        dir->y = rand() % 11 - 5;
    }if (new_y <= 0) {
        new_y = 0;
        dir->x = rand() % 11 - 5;
        dir->y = rand() % 11 - 5;
    }if (new_x >= window_w - rect->w) {
        new_x = window_w - rect->w;
        dir->x = rand() % 11 - 5;
        dir->y = rand() % 11 - 5;
    }if (new_y >= window_h - rect->h) {
        new_y = window_h - rect->h;
        dir->x = rand() % 11 - 5;
        dir->y = rand() % 11 - 5;
    }

    rect->x = new_x;
    rect->y = new_y;

    SDL_RenderFillRect(renderer, rect);

}

int main(int argc, char **argv){
    (void)argc;
    (void)argv;

    srand(time(NULL));

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    SDL_Event event;

    SDL_bool running = SDL_TRUE;


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(0, "ERROR SDL INIT", window, renderer);
    }

    window = SDL_CreateWindow(
        "Fenêtre Principal",
        0, 0,
        0, 0,
        SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == NULL ) {
        end_sdl(0, "ERROR WINDOW INIT", window, renderer);
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
        end_sdl(0, "ERROR RENDERER INIT", window, renderer);
    }

    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);

    SDL_Rect r1;
    r1.x = rand() % window_w;
    r1.y = rand() % window_h;
    r1.w = 400;
    r1.h = 400;
    Vector vector_r1;
    vector_r1.x = rand() % 11 - 5;
    vector_r1.y = rand() % 11 - 5;



    while (running){
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw(window, renderer, &r1, &vector_r1, window_w, window_h);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_Delay(10);

    end_sdl(1, "Normal ending", window, renderer);

    return 0;
}