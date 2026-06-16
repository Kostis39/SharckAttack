#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "draw.h"
#include "snake.h"
#include "window_tools.h"

static void print_controls(void) {
    printf("=== Pave de serpents ===\n");
    printf("Commandes :\n");
    printf("Espace / p : pause / reprise\n");
    printf("+          : accelerer les serpents\n");
    printf("-          : ralentir les serpents\n");
    printf("r          : reinitialiser les serpents\n");
    printf("i          : afficher position et taille de la fenetre\n");
    printf("c          : recentrer la fenetre\n");
    printf("Fleches    : deplacer la fenetre\n");
    printf("q / ESC    : quitter\n");
    printf("========================\n");
}

static void clean_sdl(SDL_Window *window, SDL_Renderer *renderer) {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

static void compute_initial_window_size(int *width, int *height) {
    int screen_width = 1000;
    int screen_height = 700;

    if (get_display_size(&screen_width, &screen_height)) {
        *width = screen_width * DEFAULT_WINDOW_RATIO_NUM / DEFAULT_WINDOW_RATIO_DEN;
        *height = screen_height * DEFAULT_WINDOW_RATIO_NUM / DEFAULT_WINDOW_RATIO_DEN;
    } else {
        *width = 1000;
        *height = 700;
    }

    if (*width < MIN_WINDOW_WIDTH) {
        *width = MIN_WINDOW_WIDTH;
    }

    if (*height < MIN_WINDOW_HEIGHT) {
        *height = MIN_WINDOW_HEIGHT;
    }
}

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;

    SDL_bool running = SDL_TRUE;
    SDL_bool paused = SDL_FALSE;

    int width = 0;
    int height = 0;
    int speed = BASE_SPEED;

    Snake snakes[SNAKE_COUNT];

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    print_display_mode();
    compute_initial_window_size(&width, &height);

    window = SDL_CreateWindow(
        "Pave de serpents",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE
    );

    if (window == NULL) {
        SDL_Log("Erreur SDL_CreateWindow : %s", SDL_GetError());
        clean_sdl(window, renderer);
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL) {
        SDL_Log("Renderer accelere impossible, tentative software : %s", SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        if (renderer == NULL) {
            SDL_Log("Erreur SDL_CreateRenderer : %s", SDL_GetError());
            clean_sdl(window, renderer);
            return EXIT_FAILURE;
        }
    }

    print_controls();
    print_window_info(window);
    init_snakes(snakes, SNAKE_COUNT, width, height);

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        width = event.window.data1;
                        height = event.window.data2;
                        printf("Nouvelle taille : w = %d, h = %d\n", width, height);
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            running = SDL_FALSE;
                            break;

                        case SDLK_SPACE:
                        case SDLK_p:
                            paused = !paused;
                            break;

                        case SDLK_r:
                            randomize_snakes(snakes, SNAKE_COUNT, width, height);
                            break;

                        case SDLK_i:
                            print_window_info(window);
                            break;

                        case SDLK_c:
                            center_window(window);
                            break;

                        case SDLK_RIGHT:
                            move_window(window, WINDOW_MOVE_STEP, 0);
                            break;

                        case SDLK_LEFT:
                            move_window(window, -WINDOW_MOVE_STEP, 0);
                            break;

                        case SDLK_UP:
                            move_window(window, 0, -WINDOW_MOVE_STEP);
                            break;

                        case SDLK_DOWN:
                            move_window(window, 0, WINDOW_MOVE_STEP);
                            break;

                        case SDLK_PLUS:
                        case SDLK_KP_PLUS:
                        case SDLK_EQUALS:
                            if (speed < MAX_SPEED) {
                                speed++;
                            }
                            printf("Vitesse : %d\n", speed);
                            break;

                        case SDLK_MINUS:
                        case SDLK_KP_MINUS:
                            if (speed > MIN_SPEED) {
                                speed--;
                            }
                            printf("Vitesse : %d\n", speed);
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        if (!paused) {
            update_snakes(snakes, SNAKE_COUNT, width, height, speed);
        }

        draw_scene(renderer, snakes, SNAKE_COUNT, width, height, paused);
        SDL_Delay(FRAME_DELAY_MS);
    }

    clean_sdl(window, renderer);

    return EXIT_SUCCESS;
}
