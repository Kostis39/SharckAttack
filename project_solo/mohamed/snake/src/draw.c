#include "draw.h"

static void draw_background(SDL_Renderer *renderer, int width, int height) {
    SDL_Rect background;

    background.x = 0;
    background.y = 0;
    background.w = width;
    background.h = height;

    SDL_SetRenderDrawColor(renderer, 10, 15, 30, 255);
    SDL_RenderClear(renderer);
    SDL_RenderFillRect(renderer, &background);
}

static void draw_grid(SDL_Renderer *renderer, int width, int height) {
    SDL_SetRenderDrawColor(renderer, 30, 40, 65, 255);

    for (int x = 0; x < width; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, height);
    }

    for (int y = 0; y < height; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, width, y);
    }
}

static void draw_filled_square(SDL_Renderer *renderer, int center_x, int center_y, int size) {
    SDL_Rect rectangle;

    rectangle.x = center_x - size / 2;
    rectangle.y = center_y - size / 2;
    rectangle.w = size;
    rectangle.h = size;

    SDL_RenderFillRect(renderer, &rectangle);
}

static void draw_snake(SDL_Renderer *renderer, Snake *snake) {
    for (int i = SNAKE_LENGTH - 1; i >= 0; i--) {
        int factor = 255 - i * 6;

        if (factor < 60) {
            factor = 60;
        }

        SDL_SetRenderDrawColor(
            renderer,
            (unsigned char)(snake->red * factor / 255),
            (unsigned char)(snake->green * factor / 255),
            (unsigned char)(snake->blue * factor / 255),
            255
        );

        draw_filled_square(renderer, snake->body[i].x, snake->body[i].y, SNAKE_SIZE);

        if (i > 0) {
            SDL_RenderDrawLine(
                renderer,
                snake->body[i].x,
                snake->body[i].y,
                snake->body[i - 1].x,
                snake->body[i - 1].y
            );
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_filled_square(renderer, snake->body[0].x, snake->body[0].y, SNAKE_SIZE + 4);
}

static void draw_pause_icon(SDL_Renderer *renderer, int width, int height) {
    SDL_Rect left_bar;
    SDL_Rect right_bar;

    left_bar.x = width / 2 - 30;
    left_bar.y = height / 2 - 40;
    left_bar.w = 18;
    left_bar.h = 80;

    right_bar.x = width / 2 + 12;
    right_bar.y = height / 2 - 40;
    right_bar.w = 18;
    right_bar.h = 80;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 210);
    SDL_RenderFillRect(renderer, &left_bar);
    SDL_RenderFillRect(renderer, &right_bar);
}

void draw_scene(SDL_Renderer *renderer, Snake snakes[], int count, int width, int height, int paused) {
    draw_background(renderer, width, height);
    draw_grid(renderer, width, height);

    for (int i = 0; i < count; i++) {
        draw_snake(renderer, &snakes[i]);
    }

    if (paused) {
        draw_pause_icon(renderer, width, height);
    }

    SDL_RenderPresent(renderer);
}
