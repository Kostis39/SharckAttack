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
    int red;
    int green;
    int blue;
    int alpha;
} Entity;

void end_sdl(
    int ok,                // fin normale : ok = 0 ; anormale ok = 1
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
    int dirx, diry;

    for (int i=0; i<nb_entity; ++i){
        dirx = rand() % 11 - 5;
        diry = rand() % 11 - 5;
        new_entity[i].hitbox.h = rand() % h + 20;
        new_entity[i].hitbox.w = rand() % w + 20;
        new_entity[i].hitbox.x = rand() % (window_w - w);
        new_entity[i].hitbox.y = rand() % (window_h - h);
        new_entity[i].dirx  = dirx ? dirx : 1;
        new_entity[i].diry  = diry ? diry : 1;
        new_entity[i].red   = rand() % 255;
        new_entity[i].green = rand() % 255;
        new_entity[i].blue  = rand() % 255;
        new_entity[i].alpha = rand() % 255;
    }

    return new_entity;
}

void entityMove(Entity * entity, Entity * list_entities, int nb_entities, int window_w, int window_h){
    int old_x = entity->hitbox.x;
    int old_y = entity->hitbox.y;
    int new_x = old_x + entity->dirx;
    int new_y = old_y + entity->diry;

    SDL_Rect futur_pos = { new_x, new_y, entity->hitbox.w, entity->hitbox.h };

    if (new_x <= 0) {
        new_x = 0;
        entity->dirx = -entity->dirx;
    }if (new_y <= 0) {
        new_y = 0;
        entity->diry = -entity->diry;
    }if (new_x >= window_w - entity->hitbox.w) {
        new_x = window_w - entity->hitbox.w;
        entity->dirx = -entity->dirx;
    }if (new_y >= window_h - entity->hitbox.h) {
        new_y = window_h - entity->hitbox.h;
        entity->diry = -entity->diry;
    }

    for (int i=0; i<nb_entities; ++i){
        if (&list_entities[i] != entity){
            if (SDL_HasIntersection(&futur_pos, &list_entities[i].hitbox)){

                SDL_Rect test_x = { new_x, old_y, entity->hitbox.w, entity->hitbox.h };
                if (SDL_HasIntersection(&test_x, &list_entities[i].hitbox)) {
                    new_x = old_x;
                    entity->dirx = -entity->dirx;
                }

                SDL_Rect test_y = { old_x, new_y, entity->hitbox.w, entity->hitbox.h };
                if (SDL_HasIntersection(&test_y, &list_entities[i].hitbox)) {
                    new_y = old_y;
                    entity->diry = -entity->diry;
                }

                futur_pos.x = new_x;
                futur_pos.y = new_y;
            }
        }
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
        entity->red, entity->green, entity->blue,
        entity->alpha
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

    Entity * list_entities = initEntity(50, 50, NUMBER_OF_ENNEMIES, window_w, window_h);

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

        for (int i=0; i<NUMBER_OF_ENNEMIES; ++i){
            entityMove(&list_entities[i], list_entities, NUMBER_OF_ENNEMIES, window_w, window_h);
        }
        for (int i=0; i<NUMBER_OF_ENNEMIES; ++i){
            drawEntity(renderer, &list_entities[i]);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_Delay(10);

    end_sdl(1, "Normal ending", window, renderer);
    free(list_entities);
    return 0;
}