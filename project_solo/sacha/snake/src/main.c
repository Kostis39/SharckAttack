#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

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

int main(int argc, char **argv){
    (void)argc;
    (void)argv;

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    SDL_Event event;

    SDL_bool running = SDL_TRUE;


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        end_sdl(0, "ERROR SDL INIT", window, renderer);
    }

    window = SDL_CreateWindow(
        "Fenêtre Principal",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        0, 0,
        SDL_WINDOW_FULLSCREEN);
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_Delay(10);

    end_sdl(1, "Normal ending", window, renderer);
    
    return 0;
}