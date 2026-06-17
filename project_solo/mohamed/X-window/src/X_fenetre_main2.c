#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define NB_WINDOWS 13

#define INITIAL_WINDOW_W 130
#define INITIAL_WINDOW_H 90
#define MIN_WINDOW_W 80
#define MIN_WINDOW_H 60

#define GAP_X 95
#define GAP_Y 65

#define MOVE_STEP 35
#define RESIZE_STEP 10
#define FRAME_DELAY_MS 20

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} AppWindow;

/*
 * Positions relatives des fenêtres.
 * Les 7 premières forment la diagonale haut-gauche -> bas-droite.
 * Les 6 dernières forment la diagonale bas-gauche -> haut-droite.
 * Le centre n'est pas répété.
 */
static const int REL_X[NB_WINDOWS] = {
    -3, -2, -1, 0, 1, 2, 3,
    -3, -2, -1, 1, 2, 3
};

static const int REL_Y[NB_WINDOWS] = {
    -3, -2, -1, 0, 1, 2, 3,
     3,  2,  1, -1, -2, -3
};

static void destroy_all(AppWindow windows[], int count) {
    for (int i = 0; i < count; i++) {
        if (windows[i].renderer != NULL) {
            SDL_DestroyRenderer(windows[i].renderer);
            windows[i].renderer = NULL;
        }

        if (windows[i].window != NULL) {
            SDL_DestroyWindow(windows[i].window);
            windows[i].window = NULL;
        }
    }

    SDL_Quit();
}

static int init_sdl(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        return 0;
    }

    return 1;
}

static int get_screen_size(int *screen_w, int *screen_h) {
    SDL_DisplayMode screen;

    if (SDL_GetCurrentDisplayMode(0, &screen) != 0) {
        SDL_Log("Erreur SDL_GetCurrentDisplayMode : %s", SDL_GetError());
        return 0;
    }

    *screen_w = screen.w;
    *screen_h = screen.h;

    printf("Resolution ecran : w = %d, h = %d\n", *screen_w, *screen_h);

    return 1;
}

static SDL_Renderer *create_renderer(SDL_Window *window) {
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL) {
        SDL_Log("Renderer accelere impossible, tentative software : %s",
                SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }

    return renderer;
}

static int create_windows(AppWindow windows[], int count) {
    char title[64];

    for (int i = 0; i < count; i++) {
        snprintf(title, sizeof(title), "X fenetre %02d", i + 1);

        windows[i].window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            INITIAL_WINDOW_W,
            INITIAL_WINDOW_H,
            SDL_WINDOW_RESIZABLE
        );

        if (windows[i].window == NULL) {
            SDL_Log("Erreur SDL_CreateWindow : %s", SDL_GetError());
            return 0;
        }

        SDL_SetWindowMinimumSize(windows[i].window, MIN_WINDOW_W, MIN_WINDOW_H);

        windows[i].renderer = create_renderer(windows[i].window);

        if (windows[i].renderer == NULL) {
            SDL_Log("Erreur SDL_CreateRenderer : %s", SDL_GetError());
            return 0;
        }
    }

    return 1;
}

static void place_x_shape(AppWindow windows[],
                          int count,
                          int screen_w,
                          int screen_h,
                          int window_w,
                          int window_h,
                          int offset_x,
                          int offset_y) {
    int center_x = screen_w / 2;
    int center_y = screen_h / 2;

    for (int i = 0; i < count; i++) {
        int x = center_x + REL_X[i] * GAP_X - window_w / 2 + offset_x;
        int y = center_y + REL_Y[i] * GAP_Y - window_h / 2 + offset_y;

        SDL_SetWindowPosition(windows[i].window, x, y);
        SDL_SetWindowSize(windows[i].window, window_w, window_h);
    }
}

static void print_windows_info(AppWindow windows[], int count) {
    printf("===== Informations des fenetres =====\n");

    for (int i = 0; i < count; i++) {
        int x;
        int y;
        int w;
        int h;

        SDL_GetWindowPosition(windows[i].window, &x, &y);
        SDL_GetWindowSize(windows[i].window, &w, &h);

        printf("Fenetre %02d : x = %d, y = %d | w = %d, h = %d\n",
               i + 1, x, y, w, h);
    }

    printf("=====================================\n");
}

static void draw_one_window(AppWindow *app_window, int index) {
    int w;
    int h;
    SDL_Rect rectangle;

    SDL_GetWindowSize(app_window->window, &w, &h);

    SDL_SetRenderDrawColor(app_window->renderer, 10, 15, 30, 255);
    SDL_RenderClear(app_window->renderer);

    rectangle.x = 10;
    rectangle.y = 10;
    rectangle.w = w - 20;
    rectangle.h = h - 20;

    if (rectangle.w < 1) {
        rectangle.w = 1;
    }

    if (rectangle.h < 1) {
        rectangle.h = 1;
    }

    SDL_SetRenderDrawColor(
        app_window->renderer,
        60 + (index * 13) % 120,
        120 + (index * 7) % 90,
        220,
        255
    );

    SDL_RenderFillRect(app_window->renderer, &rectangle);

    SDL_SetRenderDrawColor(app_window->renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(app_window->renderer, 0, 0, w, h);
    SDL_RenderDrawLine(app_window->renderer, w, 0, 0, h);

    SDL_RenderPresent(app_window->renderer);
}

static void draw_all_windows(AppWindow windows[], int count) {
    for (int i = 0; i < count; i++) {
        draw_one_window(&windows[i], i);
    }
}

static void print_commands(void) {
    printf("Commandes :\n");
    printf("Fleches  : deplacer toutes les fenetres\n");
    printf("+ / -    : agrandir / reduire toutes les fenetres\n");
    printf("i        : afficher position et taille\n");
    printf("c        : recentrer le X\n");
    printf("q / ESC  : quitter\n");
    printf("-------------------------------------\n");
}

static void handle_key(SDL_Keycode key,
                       SDL_bool *running,
                       AppWindow windows[],
                       int screen_w,
                       int screen_h,
                       int *window_w,
                       int *window_h,
                       int *offset_x,
                       int *offset_y) {
    switch (key) {
        case SDLK_ESCAPE:
        case SDLK_q:
            *running = SDL_FALSE;
            break;

        case SDLK_RIGHT:
            *offset_x += MOVE_STEP;
            break;

        case SDLK_LEFT:
            *offset_x -= MOVE_STEP;
            break;

        case SDLK_UP:
            *offset_y -= MOVE_STEP;
            break;

        case SDLK_DOWN:
            *offset_y += MOVE_STEP;
            break;

        case SDLK_PLUS:
        case SDLK_KP_PLUS:
        case SDLK_EQUALS:
            *window_w += RESIZE_STEP;
            *window_h += RESIZE_STEP;
            break;

        case SDLK_MINUS:
        case SDLK_KP_MINUS:
            if (*window_w > MIN_WINDOW_W && *window_h > MIN_WINDOW_H) {
                *window_w -= RESIZE_STEP;
                *window_h -= RESIZE_STEP;
            }
            break;

        case SDLK_c:
            *offset_x = 0;
            *offset_y = 0;
            *window_w = INITIAL_WINDOW_W;
            *window_h = INITIAL_WINDOW_H;
            break;

        case SDLK_i:
            print_windows_info(windows, NB_WINDOWS);
            break;

        default:
            break;
    }

    place_x_shape(
        windows,
        NB_WINDOWS,
        screen_w,
        screen_h,
        *window_w,
        *window_h,
        *offset_x,
        *offset_y
    );
}

int main(void) {
    AppWindow windows[NB_WINDOWS] = {0};
    SDL_Event event;
    SDL_bool running = SDL_TRUE;

    int screen_w = 0;
    int screen_h = 0;

    int window_w = INITIAL_WINDOW_W;
    int window_h = INITIAL_WINDOW_H;

    int offset_x = 0;
    int offset_y = 0;

    if (!init_sdl()) {
        return EXIT_FAILURE;
    }

    if (!get_screen_size(&screen_w, &screen_h)) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (!create_windows(windows, NB_WINDOWS)) {
        destroy_all(windows, NB_WINDOWS);
        return EXIT_FAILURE;
    }

    place_x_shape(
        windows,
        NB_WINDOWS,
        screen_w,
        screen_h,
        window_w,
        window_h,
        offset_x,
        offset_y
    );

    print_commands();
    print_windows_info(windows, NB_WINDOWS);

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                        running = SDL_FALSE;
                    }
                    break;

                case SDL_KEYDOWN:
                    handle_key(
                        event.key.keysym.sym,
                        &running,
                        windows,
                        screen_w,
                        screen_h,
                        &window_w,
                        &window_h,
                        &offset_x,
                        &offset_y
                    );
                    break;

                default:
                    break;
            }
        }

        draw_all_windows(windows, NB_WINDOWS);
        SDL_Delay(FRAME_DELAY_MS);
    }

    destroy_all(windows, NB_WINDOWS);

    return EXIT_SUCCESS;
}