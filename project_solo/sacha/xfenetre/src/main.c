#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    SDL_Window *main_window;
    int posx;
    int posy;
    int height;
    int width;
    float speed;
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
    new_window->speed = 1;
    return new_window;
}

int moveWindow(game_window * window, int max_x, int max_y, int add_x, int add_y)
{
    if (window == NULL){
        return 0;
    }

    int new_x = window->posx + add_x * window->speed;
    int new_y = window->posy + add_y * window->speed;

    if (new_x < 0) {
        new_x = 0;
    }if (new_y < 0) {
        new_y = 0;
    }if (new_x > max_x - window->width) {
        new_x = max_x - window->width;
    }if (new_y > max_y - window->height) {
        new_y = max_y - window->height;
    }

    window->posx = new_x;
    window->posy = new_y;

    return 1;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    SDL_bool running = SDL_TRUE;

    SDL_Event event;

    SDL_DisplayMode my_screen;

    game_window * window = initWindow();
    window->height = 300;
    window->width = 400;
    

    /* Initialisation de la SDL + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError());            // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    if (SDL_GetCurrentDisplayMode(0, &my_screen) != 0) {
        SDL_Log("Error : SDL get display mode - %s\n",
                SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* Création de la fenêtre principal */
    window->main_window = SDL_CreateWindow(
        "Fenêtre Principal",              // codage en utf8, donc accents possibles
        window->posx, window->posy,
        window->width, window->height,
        SDL_WINDOW_BORDERLESS);             // redimensionnable

    if (window->main_window == NULL) {
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
                        case SDLK_RSHIFT:
                            window->speed *= (window->speed*1.5 >= 15) ? 1 : 1.5;
                            break;
                        case SDLK_LSHIFT:
                            window->speed /= (window->speed / 1.5 <= 4) ? 1 : 1.5;
                            break;
                        case SDLK_z:
                            moveWindow(window, my_screen.w, my_screen.h, 0, -1);
                            break;
                        case SDLK_s:
                            moveWindow(window, my_screen.w, my_screen.h, 0, 1);
                            break;
                        case SDLK_q:
                            moveWindow(window, my_screen.w, my_screen.h, -1, 0);
                            break;
                        case SDLK_d:
                            moveWindow(window, my_screen.w, my_screen.h, 1, 0);
                            break;
                        case SDLK_ESCAPE:
                            running = SDL_FALSE;
                            break;
                        default:
                            break;
                    }
                    SDL_SetWindowPosition(window->main_window, window->posx, window->posy);
                    printf("%d, %d, s=%f \n", window->posx, window->posy, window->speed);
                    break;
                default:
                    break;
                }
        }
    }

    SDL_Delay(20);

    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(window->main_window);
    free(window);

    SDL_Quit();

    return 0;
}
