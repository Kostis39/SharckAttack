#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 600
#define MOVE_STEP 40
#define RESIZE_W_STEP 80
#define RESIZE_H_STEP 50

static void print_window_info(SDL_Window *window) {
    int x, y, w, h;

    SDL_GetWindowPosition(window, &x, &y);
    SDL_GetWindowSize(window, &w, &h);

    printf("Position fenetre : x = %d, y = %d\n", x, y);
    printf("Taille fenetre   : w = %d, h = %d\n", w, h);
    printf("---------------------------------\n");
}

static void draw_thick_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
    for (int i = -3; i <= 3; i++) {
        SDL_RenderDrawLine(renderer, x1 + i, y1, x2 + i, y2);
        SDL_RenderDrawLine(renderer, x1, y1 + i, x2, y2 + i);
    }
}

static void draw_x(SDL_Renderer *renderer, int width, int height, int square_x, int square_y) {
    SDL_Rect moving_square;

    SDL_SetRenderDrawColor(renderer, 15, 23, 42, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 56, 189, 248, 255);

    draw_thick_line(renderer, 50, 50, width - 50, height - 50);
    draw_thick_line(renderer, width - 50, 50, 50, height - 50);

    moving_square.x = square_x;
    moving_square.y = square_y;
    moving_square.w = 60;
    moving_square.h = 60;

    SDL_SetRenderDrawColor(renderer, 239, 68, 68, 255);
    SDL_RenderFillRect(renderer, &moving_square);

    SDL_RenderPresent(renderer);
}

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_DisplayMode screen;
    SDL_Event event;

    SDL_bool running = SDL_TRUE;
    int width = INITIAL_WIDTH;
    int height = INITIAL_HEIGHT;
    int square_x = 100;
    int square_y = 100;
    int square_size = 60;

    int square_vx = 4;
    int square_vy = 3;  

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (SDL_GetCurrentDisplayMode(0, &screen) != 0) {
        SDL_Log("Erreur SDL_GetCurrentDisplayMode : %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    printf("Resolution ecran : w = %d, h = %d\n", screen.w, screen.h);

    window = SDL_CreateWindow(
        "X fenetre",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE
    );

    if (window == NULL) {
        SDL_Log("Erreur SDL_CreateWindow : %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL) {
        SDL_Log("Renderer accelere impossible, tentative en software : %s", SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        if (renderer == NULL) {
            SDL_Log("Erreur SDL_CreateRenderer : %s", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }
    }

    printf("Commandes :\n");
    printf("Fleches : deplacer la fenetre\n");
    printf("+ / -   : agrandir / reduire la fenetre\n");
    printf("i       : afficher position et taille\n");
    printf("c       : recentrer la fenetre\n");
    printf("q / ESC : quitter\n");
    printf("---------------------------------\n");

    print_window_info(window);

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

                case SDL_KEYDOWN: {
                    int x, y, w, h;
                    SDL_GetWindowPosition(window, &x, &y);
                    SDL_GetWindowSize(window, &w, &h);

                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            running = SDL_FALSE;
                            break;

                        case SDLK_RIGHT:
                            SDL_SetWindowPosition(window, x + MOVE_STEP, y);
                            print_window_info(window);
                            break;

                        case SDLK_LEFT:
                            SDL_SetWindowPosition(window, x - MOVE_STEP, y);
                            print_window_info(window);
                            break;

                        case SDLK_UP:
                            SDL_SetWindowPosition(window, x, y - MOVE_STEP);
                            print_window_info(window);
                            break;

                        case SDLK_DOWN:
                            SDL_SetWindowPosition(window, x, y + MOVE_STEP);
                            print_window_info(window);
                            break;

                        case SDLK_PLUS:
                        case SDLK_KP_PLUS:
                        case SDLK_EQUALS:
                            SDL_SetWindowSize(window, w + RESIZE_W_STEP, h + RESIZE_H_STEP);
                            SDL_GetWindowSize(window, &width, &height);
                            print_window_info(window);
                            break;

                        case SDLK_MINUS:
                        case SDLK_KP_MINUS:
                            if (w > 250 && h > 200) {
                                SDL_SetWindowSize(window, w - RESIZE_W_STEP, h - RESIZE_H_STEP);
                                SDL_GetWindowSize(window, &width, &height);
                                print_window_info(window);
                            }
                            break;

                        case SDLK_i:
                            print_window_info(window);
                            break;

                        case SDLK_c:
                            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                            print_window_info(window);
                            break;

                        default:
                            break;
                    }
                    break;
                }

                default:
                    break;
            }
        }
        square_x += square_vx;
        square_y += square_vy;

        if (square_x <= 0 || square_x + square_size >= width) {
            square_vx = -square_vx;
    }

        if (square_y <= 0 || square_y + square_size >= height) {
            square_vy = -square_vy;
    }  

        draw_x(renderer, width, height, square_x, square_y);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}