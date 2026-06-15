#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    SDL_Window *main_window;
    int posx;
    int posy;
    int height;
    int width;
} game_window;

game_window * initWindow(){
    game_window * new_window = malloc(sizeof(game_window));
    if (new_window == NULL) {
        return NULL;
    }
    new_window->main_window = NULL;
    new_window->posx = 0;
    new_window->posy = 0;
    new_window->height = 0;
    new_window->width = 0;
    return new_window;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    SDL_bool running = SDL_TRUE;
    SDL_bool paused = SDL_FALSE;

    SDL_Event event;

    SDL_Window *main_window = NULL;
    

    /* Initialisation de la SDL + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError());            // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    /* Création de la fenêtre de gauche */
    main_window = SDL_CreateWindow(
        "Fenêtre à gauche",              // codage en utf8, donc accents possibles
        0, 0,                              // coin haut gauche en haut gauche de l'écran
        400, 300,                          // largeur = 400, hauteur = 300
        SDL_WINDOW_RESIZABLE);             // redimensionnable

    if (main_window == NULL) {
        SDL_Log("Error : SDL main window creation - %s\n",
                SDL_GetError());             // échec de la création de la fenêtre
        SDL_Quit();                        // On referme la SDL
        exit(EXIT_FAILURE);
    }

    while (running){
        while(SDL_PollEvent(&event)){

            switch (event.type){
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = SDL_FALSE;
                        default:
                            break;
                    }
                default:
                    break;
                }
        }
    }

    SDL_Delay(50);

    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(main_window);

    SDL_Quit();

    return 0;
}
