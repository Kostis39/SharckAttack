#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NUMBER_OF_ENNEMIES 8

typedef struct{
    SDL_Rect hitbox;
    float dirx;
    float diry;
} Entity;

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

Entity * initEntity(int w, int h, int nb_entity, int window_w, int window_h){
    Entity * new_entity = calloc(nb_entity, sizeof(Entity));

    for (int i=0; i<nb_entity; ++i){
        new_entity[i].hitbox.h = h;
        new_entity[i].hitbox.w = w;
        new_entity[i].hitbox.x = rand() % (window_w - w);
        new_entity[i].hitbox.y = rand() % (window_h - h);
        new_entity[i].dirx = rand() % 11 - 5;
        new_entity[i].diry = rand() % 11 - 5;
    }

    return new_entity;
}

void entityMove(Entity * entity, int window_w, int window_h){
    int new_x = entity->hitbox.x + entity->dirx;
    int new_y = entity->hitbox.y + entity->diry;
    
    if (new_x <= 0) {
        new_x = 0;
        entity->dirx = rand() % 11 - 5;
        entity->diry = rand() % 11 - 5;
    }if (new_y <= 0) {
        new_y = 0;
        entity->dirx = rand() % 11 - 5;
        entity->diry = rand() % 11 - 5;
    }if (new_x >= window_w - entity->hitbox.w) {
        new_x = window_w - entity->hitbox.w;
        entity->dirx = rand() % 11 - 5;
        entity->diry = rand() % 11 - 5;
    }if (new_y >= window_h - entity->hitbox.h) {
        new_y = window_h - entity->hitbox.h;
        entity->dirx = rand() % 11 - 5;
        entity->diry = rand() % 11 - 5;
    }

    entity->hitbox.x = new_x;
    entity->hitbox.y = new_y;
}

void drawEntity(SDL_Renderer * renderer, Entity * entity){
    if (renderer == NULL){
        return;
    }
    SDL_SetRenderDrawColor(
        renderer,
        250, 0, 0,
        255
    );

    SDL_RenderFillRect(renderer, &(entity->hitbox));

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

    Entity * auto_player = initEntity(400, 400, 1, window_w, window_h);

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

        entityMove(auto_player, window_w, window_h);
        drawEntity(renderer, auto_player);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_Delay(10);

    end_sdl(1, "Normal ending", window, renderer);
    free(auto_player);

    return 0;
}