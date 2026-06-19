#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_WINDOWS 8

typedef struct {
    SDL_Window *main_window;
    float posx, posy;
    int height, width;
    float speed;
    SDL_bool key_up, key_down, key_left, key_right;
} game_window;

game_window *initWindow() {
    game_window *new_window = malloc(sizeof(game_window));
    if (new_window == NULL) {
        return NULL;
    }
    new_window->main_window = NULL;
    new_window->posx = 0.0f;
    new_window->posy = 0.0f;
    new_window->height = 0;
    new_window->width = 0;
    new_window->speed = 10.0f;
    new_window->key_up = SDL_FALSE;
    new_window->key_down = SDL_FALSE;
    new_window->key_left = SDL_FALSE;
    new_window->key_right = SDL_FALSE;

    return new_window;
}

int moveWindow(game_window *window, int screen_w, int screen_h) {
    if (window == NULL) {
        return 0;
    }

    int reel_x;
    int reel_y;

    int dirx = (window->key_right && !window->key_left)   ? 1
               : (window->key_left && !window->key_right) ? -1
                                                          : 0;
    int diry = (window->key_down && !window->key_up)   ? 1
               : (window->key_up && !window->key_down) ? -1
                                                       : 0;

    SDL_GetWindowPosition(window->main_window, &reel_x, &reel_y);
    if (reel_x != (int)window->posx)
        window->posx = reel_x;
    if (reel_y != (int)window->posy)
        window->posy = reel_y;

    float lenght = sqrt(dirx * dirx + diry * diry);
    if (lenght != 0) {
        float new_x = window->posx + dirx / lenght * window->speed;
        float new_y = window->posy + diry / lenght * window->speed;

        if (new_x < 0) {
            new_x = 0;
        }
        if (new_y < 0) {
            new_y = 0;
        }
        if (new_x > screen_w - window->width) {
            new_x = screen_w - window->width;
        }
        if (new_y > screen_h - window->height) {
            new_y = screen_h - window->height;
        }

        window->posx = new_x;
        window->posy = new_y;

        SDL_SetWindowPosition(window->main_window, (int)new_x, (int)new_y);
    }

    return 1;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SDL_bool running = SDL_TRUE;

    SDL_Event event;

    SDL_DisplayMode my_screen;

    game_window *window = initWindow();
    window->height = 32;
    window->width = 400;

    SDL_Window *lst_window[MAX_WINDOWS] = {NULL};
    int windowCount = 0;

    /* Initialisation de la SDL + gestion de l'échec possible */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error : SDL initialisation - %s\n",
                SDL_GetError()); // l'initialisation de la SDL a échoué
        exit(EXIT_FAILURE);
    }

    if (SDL_GetCurrentDisplayMode(0, &my_screen) != 0) {
        SDL_Log("Error : SDL get display mode - %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    /* Création de la fenêtre principal */
    window->main_window = SDL_CreateWindow(
        "Fenêtre Principal", window->posx, window->posy, window->width,
        window->height, SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE);

    if (window->main_window == NULL) {
        SDL_Log("Error : SDL main window creation - %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    while (running) {
        while (SDL_PollEvent(&event)) {

            switch (event.type) {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = SDL_FALSE;
                    break;
                case SDLK_1:
                    if (windowCount < MAX_WINDOWS) {
                        char title[50];
                        sprintf(title, "Fenêtre %d", windowCount + 1);
                        lst_window[windowCount] = SDL_CreateWindow(
                            title, window->posx, window->posy, window->width,
                            window->height, SDL_WINDOW_SHOWN);
                        windowCount++;
                    }
                    break;
                case SDLK_2:
                    if (windowCount > 0) {
                        windowCount--;
                        SDL_DestroyWindow(lst_window[windowCount]);
                        lst_window[windowCount] = NULL;
                    }
                    break;
                case SDLK_RSHIFT:
                    window->speed *= (window->speed * 1.5 >= 60) ? 1 : 1.5;
                    break;
                case SDLK_LSHIFT:
                    window->speed /= (window->speed / 1.5 <= 4) ? 1 : 1.5;
                    break;
                case SDLK_a:
                    window->width += 100;
                    window->height += 100;
                    SDL_SetWindowSize(window->main_window, window->width,
                                      window->height);
                    break;
                case SDLK_e:
                    window->width =
                        (window->width - 100 > 100) ? window->width - 100 : 100;
                    window->height = (window->height - 100 > 100)
                                         ? window->height - 100
                                         : 100;
                    SDL_SetWindowSize(window->main_window, window->width,
                                      window->height);
                    break;
                case SDLK_z:
                    window->key_up = SDL_TRUE;
                    break;
                case SDLK_s:
                    window->key_down = SDL_TRUE;
                    break;
                case SDLK_q:
                    window->key_left = SDL_TRUE;
                    break;
                case SDLK_d:
                    window->key_right = SDL_TRUE;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                case SDLK_z:
                    window->key_up = SDL_FALSE;
                    break;
                case SDLK_s:
                    window->key_down = SDL_FALSE;
                    break;
                case SDLK_q:
                    window->key_left = SDL_FALSE;
                    break;
                case SDLK_d:
                    window->key_right = SDL_FALSE;
                    break;
                case SDLK_ESCAPE:
                    running = SDL_FALSE;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        moveWindow(window, my_screen.w, my_screen.h);
        SDL_Delay(5);
    }

    for (int i = 0; i < windowCount; i++) {
        if (lst_window[i])
            SDL_DestroyWindow(lst_window[i]);
    }
    SDL_DestroyWindow(window->main_window);
    free(window);

    SDL_Quit();

    return 0;
}
