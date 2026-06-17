#include "scene.h"

static void draw_rect_layer(SDL_Renderer *renderer,
                            int scroll_x,
                            int speed_divider,
                            int base_y,
                            int height,
                            int r,
                            int g,
                            int b) {
    int offset = (scroll_x / speed_divider) % WINDOW_WIDTH;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    for (int x = -offset; x < WINDOW_WIDTH; x += 140) {
        SDL_Rect block = {
            x,
            base_y - height,
            90,
            height
        };

        SDL_RenderFillRect(renderer, &block);

        SDL_Rect block2 = {
            x + 70,
            base_y - height - 35,
            65,
            height + 35
        };

        SDL_RenderFillRect(renderer, &block2);
    }
}

void draw_background(SDL_Renderer *renderer,
                     SDL_Texture *background,
                     int scroll_x) {
    if (background != NULL) {
        SDL_Rect src = {0};
        SDL_Rect dst = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

        SDL_QueryTexture(background, NULL, NULL, &src.w, &src.h);

        if (src.w > WINDOW_WIDTH) {
            src.x = (scroll_x / 10) % (src.w - WINDOW_WIDTH);
            src.w = WINDOW_WIDTH;
        }

        if (src.h > WINDOW_HEIGHT) {
            src.y = 100;
            src.h = WINDOW_HEIGHT;
        }

        SDL_RenderCopy(renderer, background, &src, &dst);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
        SDL_RenderFillRect(renderer, &dst);
    } else {
        SDL_SetRenderDrawColor(renderer, 20, 25, 45, 255);
        SDL_RenderClear(renderer);
    }

    /* Lune : élément très lointain, donc fixe */
    SDL_SetRenderDrawColor(renderer, 240, 220, 120, 255);
    SDL_Rect moon = {WINDOW_WIDTH - 120, 60, 50, 50};
    SDL_RenderFillRect(renderer, &moon);

    /* Étoiles : bougent très lentement */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);

    for (int i = 0; i < 35; i++) {
        int x = ((i * 91) - scroll_x / 12) % WINDOW_WIDTH;
        int y = 40 + (i * 47) % 250;

        if (x < 0) {
            x += WINDOW_WIDTH;
        }

        SDL_RenderDrawPoint(renderer, x, y);
    }

    /*
        Parallaxe :
        - couche lointaine : vitesse lente
        - couche moyenne : vitesse moyenne
        - couche proche : vitesse rapide
    */

    draw_rect_layer(renderer,
                    scroll_x,
                    10,
                    WINDOW_HEIGHT - GROUND_HEIGHT,
                    120,
                    30,
                    45,
                    70);

    draw_rect_layer(renderer,
                    scroll_x,
                    5,
                    WINDOW_HEIGHT - GROUND_HEIGHT,
                    160,
                    45,
                    70,
                    100);

    draw_rect_layer(renderer,
                    scroll_x,
                    2,
                    WINDOW_HEIGHT - GROUND_HEIGHT,
                    90,
                    25,
                    35,
                    50);
}

void draw_ground(SDL_Renderer *renderer) {
    SDL_Rect ground = {
        0,
        WINDOW_HEIGHT - GROUND_HEIGHT,
        WINDOW_WIDTH,
        GROUND_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 45, 45, 55, 255);
    SDL_RenderFillRect(renderer, &ground);

    SDL_Rect line = {
        0,
        WINDOW_HEIGHT - GROUND_HEIGHT,
        WINDOW_WIDTH,
        8
    };

    SDL_SetRenderDrawColor(renderer, 130, 130, 150, 255);
    SDL_RenderFillRect(renderer, &line);

    SDL_SetRenderDrawColor(renderer, 70, 70, 85, 255);

    for (int x = 0; x < WINDOW_WIDTH; x += 80) {
        SDL_Rect brick = {
            x,
            WINDOW_HEIGHT - GROUND_HEIGHT + 25,
            50,
            10
        };

        SDL_RenderFillRect(renderer, &brick);
    }
}