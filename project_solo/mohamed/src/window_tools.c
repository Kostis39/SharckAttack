#include "window_tools.h"

#include <stdio.h>

int get_display_size(int *width, int *height) {
    SDL_DisplayMode screen;

    if (SDL_GetCurrentDisplayMode(0, &screen) != 0) {
        SDL_Log("Erreur SDL_GetCurrentDisplayMode : %s", SDL_GetError());
        return 0;
    }

    *width = screen.w;
    *height = screen.h;

    return 1;
}

void print_display_mode(void) {
    int screen_width = 0;
    int screen_height = 0;

    if (get_display_size(&screen_width, &screen_height)) {
        printf("Resolution ecran : w = %d, h = %d\n", screen_width, screen_height);
    }
}

void print_window_info(SDL_Window *window) {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    SDL_GetWindowPosition(window, &x, &y);
    SDL_GetWindowSize(window, &width, &height);

    printf("Position fenetre : x = %d, y = %d\n", x, y);
    printf("Taille fenetre   : w = %d, h = %d\n", width, height);
    printf("---------------------------------\n");
}

void move_window(SDL_Window *window, int dx, int dy) {
    int x = 0;
    int y = 0;

    SDL_GetWindowPosition(window, &x, &y);
    SDL_SetWindowPosition(window, x + dx, y + dy);
    print_window_info(window);
}

void center_window(SDL_Window *window) {
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    print_window_info(window);
}
