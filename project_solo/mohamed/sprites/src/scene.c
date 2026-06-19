#include "scene.h"

static void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static int edge_cross_x(int y, int x1, int y1, int x2, int y2) {
    if (y1 == y2) {
        return x1;
    }

    return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
}

static void fill_triangle(SDL_Renderer *renderer,
                          int x1, int y1,
                          int x2, int y2,
                          int x3, int y3) {
    int min_y = y1;
    int max_y = y1;

    if (y2 < min_y) min_y = y2;
    if (y3 < min_y) min_y = y3;
    if (y2 > max_y) max_y = y2;
    if (y3 > max_y) max_y = y3;

    for (int y = min_y; y <= max_y; y++) {
        int xs[3];
        int count = 0;

        if ((y >= y1 && y < y2) || (y >= y2 && y < y1)) {
            xs[count++] = edge_cross_x(y, x1, y1, x2, y2);
        }

        if ((y >= y2 && y < y3) || (y >= y3 && y < y2)) {
            xs[count++] = edge_cross_x(y, x2, y2, x3, y3);
        }

        if ((y >= y3 && y < y1) || (y >= y1 && y < y3)) {
            xs[count++] = edge_cross_x(y, x3, y3, x1, y1);
        }

        if (count >= 2) {
            if (xs[0] > xs[1]) {
                swap_int(&xs[0], &xs[1]);
            }

            SDL_RenderDrawLine(renderer, xs[0], y, xs[1], y);
        }
    }
}

static void draw_sky(SDL_Renderer *renderer) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        int r = 8 + y / 80;
        int g = 14 + y / 45;
        int b = 28 + y / 18;

        if (b > 80) {
            b = 80;
        }

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, WINDOW_WIDTH, y);
    }
}

static void draw_stars_and_snow(SDL_Renderer *renderer, int scroll_x) {
    SDL_SetRenderDrawColor(renderer, 220, 235, 255, 180);

    for (int i = 0; i < 45; i++) {
        int x = ((i * 97) - scroll_x / 8) % WINDOW_WIDTH;
        int y = 30 + (i * 53) % 330;

        if (x < 0) {
            x += WINDOW_WIDTH;
        }

        SDL_RenderDrawPoint(renderer, x, y);
    }

    SDL_SetRenderDrawColor(renderer, 230, 245, 255, 120);

    for (int i = 0; i < 55; i++) {
        int x = ((i * 71) - scroll_x / 3) % WINDOW_WIDTH;
        int y = 40 + ((i * 43 + scroll_x / 2) % (WINDOW_HEIGHT - 80));

        if (x < 0) {
            x += WINDOW_WIDTH;
        }

        SDL_RenderDrawPoint(renderer, x, y);
        SDL_RenderDrawPoint(renderer, x + 1, y);
    }
}

static void draw_mountain_layer(SDL_Renderer *renderer,
                                int scroll_x,
                                int speed_divider,
                                int base_y,
                                int height,
                                int r,
                                int g,
                                int b) {
    int offset = (scroll_x / speed_divider) % 320;

    if (offset < 0) {
        offset += 320;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    for (int x = -offset - 320; x < WINDOW_WIDTH + 320; x += 320) {
        fill_triangle(renderer,
                      x,
                      base_y,
                      x + 160,
                      base_y - height,
                      x + 340,
                      base_y);

        SDL_SetRenderDrawColor(renderer, r + 20, g + 25, b + 30, 120);
        fill_triangle(renderer,
                      x + 115,
                      base_y - height + 45,
                      x + 160,
                      base_y - height,
                      x + 205,
                      base_y - height + 45);

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    }
}

static void draw_pine(SDL_Renderer *renderer, int x, int base_y, int size) {
    SDL_Rect trunk = {
        x - size / 10,
        base_y - size / 3,
        size / 5,
        size / 3
    };

    SDL_SetRenderDrawColor(renderer, 28, 22, 26, 255);
    SDL_RenderFillRect(renderer, &trunk);

    SDL_SetRenderDrawColor(renderer, 12, 42, 55, 255);
    fill_triangle(renderer,
                  x - size / 2,
                  base_y - size / 4,
                  x,
                  base_y - size,
                  x + size / 2,
                  base_y - size / 4);

    SDL_SetRenderDrawColor(renderer, 18, 55, 70, 255);
    fill_triangle(renderer,
                  x - size / 2,
                  base_y - size / 2,
                  x,
                  base_y - size - size / 4,
                  x + size / 2,
                  base_y - size / 2);

    SDL_SetRenderDrawColor(renderer, 210, 225, 235, 150);
    SDL_RenderDrawLine(renderer,
                       x - size / 4,
                       base_y - size / 2,
                       x,
                       base_y - size - size / 5);
}

static void draw_forest_layer(SDL_Renderer *renderer,
                              int scroll_x,
                              int speed_divider,
                              int base_y) {
    int offset = (scroll_x / speed_divider) % 180;

    if (offset < 0) {
        offset += 180;
    }

    for (int x = -offset - 180; x < WINDOW_WIDTH + 180; x += 180) {
        draw_pine(renderer, x + 20, base_y, 95);
        draw_pine(renderer, x + 95, base_y + 10, 120);
        draw_pine(renderer, x + 160, base_y, 85);
    }
}

void draw_background(SDL_Renderer *renderer,
                     SDL_Texture *background,
                     int scroll_x) {
    (void)background;

    draw_sky(renderer);

    SDL_SetRenderDrawColor(renderer, 235, 240, 230, 255);
    SDL_Rect moon = {WINDOW_WIDTH - 145, 65, 52, 52};
    SDL_RenderFillRect(renderer, &moon);

    draw_stars_and_snow(renderer, scroll_x);

    draw_mountain_layer(renderer,
                        scroll_x,
                        12,
                        WINDOW_HEIGHT - GROUND_HEIGHT + 10,
                        170,
                        28,
                        50,
                        80);

    draw_mountain_layer(renderer,
                        scroll_x,
                        7,
                        WINDOW_HEIGHT - GROUND_HEIGHT + 25,
                        125,
                        40,
                        70,
                        95);

    draw_forest_layer(renderer,
                      scroll_x,
                      3,
                      WINDOW_HEIGHT - GROUND_HEIGHT + 5);

    SDL_SetRenderDrawColor(renderer, 210, 225, 235, 35);
    SDL_Rect fog = {0, WINDOW_HEIGHT - GROUND_HEIGHT - 45, WINDOW_WIDTH, 55};
    SDL_RenderFillRect(renderer, &fog);
}

void draw_ground(SDL_Renderer *renderer) {
    SDL_Rect ground = {
        0,
        WINDOW_HEIGHT - GROUND_HEIGHT,
        WINDOW_WIDTH,
        GROUND_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 205, 215, 225, 255);
    SDL_RenderFillRect(renderer, &ground);

    SDL_Rect shadow = {
        0,
        WINDOW_HEIGHT - GROUND_HEIGHT + 12,
        WINDOW_WIDTH,
        GROUND_HEIGHT - 12
    };

    SDL_SetRenderDrawColor(renderer, 160, 170, 185, 255);
    SDL_RenderFillRect(renderer, &shadow);

    SDL_SetRenderDrawColor(renderer, 235, 245, 255, 255);

    for (int x = 0; x < WINDOW_WIDTH; x += 95) {
        SDL_RenderDrawLine(renderer,
                           x,
                           WINDOW_HEIGHT - GROUND_HEIGHT + 8,
                           x + 60,
                           WINDOW_HEIGHT - GROUND_HEIGHT + 5);
    }
}
