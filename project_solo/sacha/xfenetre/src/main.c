#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    SDL_Window *main_window;
    int dirx;
    int diry;
    float posx;
    float posy;
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
    new_window->dirx = 0;
    new_window->diry = 0;
    new_window->posx = 0.0f;
    new_window->posy = 0.0f;
    new_window->height = 0;
    new_window->width = 0;
    new_window->speed = 10.0f;
    return new_window;
}

int moveWindow(game_window * window, int screen_w, int screen_h)
{
    if (window == NULL){
        return 0;
    }

    int reel_x;
    int reel_y;
    SDL_GetWindowPosition(window->main_window, &reel_x, &reel_y);
    if (reel_x != (int) window->posx) window->posx = reel_x;
    if (reel_y != (int) window->posy) window->posy = reel_y;

    float lenght = sqrt(pow(window->dirx, 2)+pow(window->diry, 2)) * window->speed;
    
    float new_x = window->posx + lenght*window->dirx;
    float new_y = window->posy + lenght*window->diry;

    if (new_x < 0) {
        new_x = 0;
    }if (new_y < 0) {
        new_y = 0;
    }if (new_x > screen_w - window->width) {
        new_x = screen_w - window->width;
    }if (new_y > screen_h - window->height) {
        new_y = screen_h - window->height;
    }

    window->posx = new_x;
    window->posy = new_y;

    SDL_SetWindowPosition(window->main_window, (int) new_x, (int) new_y);

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
    window->height = 32;
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
        window->dirx, window->diry,
        window->width, window->height,
        SDL_WINDOW_BORDERLESS);


    if (window->main_window == NULL ) {
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
                            window->diry = -1;
                            break;
                        case SDLK_s:
                            window->diry = 1;
                            break;
                        case SDLK_q:
                            window->dirx = -1;
                            break;
                        case SDLK_d:
                            window->dirx = 1;
                            break;
                        case SDLK_ESCAPE:
                            running = SDL_FALSE;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_z:
                            window->diry = 0;
                            break;
                        case SDLK_s:
                            window->diry = 0;
                            break;
                        case SDLK_q:
                            window->dirx = 0;
                            break;
                        case SDLK_d:
                            window->dirx = 0;
                            break;
                        case SDLK_ESCAPE:
                            running = SDL_FALSE;
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        moveWindow(window, my_screen.w, my_screen.h);
    }

    SDL_Delay(20);

    /* et on referme tout ce qu'on a ouvert en ordre inverse de la création */
    SDL_DestroyWindow(window->main_window);
    free(window);

    SDL_Quit();

    return 0;
}
