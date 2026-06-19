#include "render_sdl.h"
#include "vector.h"

#include <math.h>
#include <stdio.h>

static int to_int(float x) {
    return (int)(x + 0.5f);
}

static Vector direction_or_default(Vector v) {
    Vector d = Vector_normalize(v);

    if (d.x == 0 && d.y == 0) {
        d.x = 1;
        d.y = 0;
    }

    return d;
}

static void draw_filled_circle(SDL_Renderer *r, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        int limit = (int)sqrtf((float)(radius * radius - y * y));
        SDL_RenderDrawLine(r, cx - limit, cy + y, cx + limit, cy + y);
    }
}

static void draw_filled_ellipse(SDL_Renderer *r, int cx, int cy, int rx, int ry) {
    if (rx <= 0 || ry <= 0)
        return;

    for (int y = -ry; y <= ry; y++) {
        float t = 1.0f - (float)(y * y) / (float)(ry * ry);
        if (t < 0.0f)
            t = 0.0f;

        int limit = (int)(rx * sqrtf(t));
        SDL_RenderDrawLine(r, cx - limit, cy + y, cx + limit, cy + y);
    }
}

static void draw_filled_triangle(SDL_Renderer *r,
                                 int x1, int y1,
                                 int x2, int y2,
                                 int x3, int y3) {
    for (int i = 0; i <= 24; i++) {
        float t = i / 24.0f;

        int ax = to_int(x1 + (x2 - x1) * t);
        int ay = to_int(y1 + (y2 - y1) * t);

        SDL_RenderDrawLine(r, ax, ay, x3, y3);
    }
}

static void draw_wave(SDL_Renderer *r, int width, int base_y,
                      int move, int amplitude, float time) {
    int old_x = -20;
    int old_y = base_y;

    for (int x = -20; x <= width + 20; x += 18) {
        int y = base_y + to_int(sinf((x + move) * 0.018f + time) * amplitude);
        SDL_RenderDrawLine(r, old_x, old_y, x, y);
        old_x = x;
        old_y = y;
    }
}

static void draw_bubbles(SDL_Renderer *r, int width, int height) {
    static int bx[14] = {
        60, 180, 310, 430, 520, 670, 790,
        910, 1040, 1160, 1260, 1350, 250, 740
    };

    static int by[14] = {
        500, 300, 620, 180, 430, 540, 250,
        610, 350, 470, 200, 570, 720, 80
    };

    if (width <= 0 || height <= 0)
        return;

    SDL_SetRenderDrawColor(r, 120, 210, 255, 90);

    for (int i = 0; i < 14; i++) {
        by[i] -= 1 + (i % 2);

        if (by[i] < -10) {
            by[i] = height + 10;
            bx[i] = (bx[i] + 173) % width;
        }

        draw_filled_circle(r, bx[i], by[i], 2 + (i % 3));
    }
}

static void draw_sea_plants(SDL_Renderer *r, int width, int height, float time) {
    SDL_SetRenderDrawColor(r, 35, 120, 125, 130);

    for (int i = 0; i < 7; i++) {
        int x = 45 + i * 24;
        int top = height - 75 - (i % 3) * 30;
        int wind = to_int(sinf(time * 1.5f + i) * 7.0f);

        SDL_RenderDrawLine(r, x, height, x + wind, top);
        SDL_RenderDrawLine(r, x + 1, height, x + wind + 1, top);
    }

    for (int i = 0; i < 7; i++) {
        int x = width - 180 + i * 24;
        int top = height - 70 - (i % 4) * 28;
        int wind = to_int(sinf(time * 1.2f + i) * 7.0f);

        SDL_RenderDrawLine(r, x, height, x + wind, top);
        SDL_RenderDrawLine(r, x + 1, height, x + wind + 1, top);
    }

    SDL_SetRenderDrawColor(r, 115, 75, 160, 145);

    for (int i = 0; i < 4; i++) {
        int x = 310 + i * 32;
        int y = height - 45;

        SDL_RenderDrawLine(r, x, y, x, y - 45);
        SDL_RenderDrawLine(r, x, y - 22, x - 14, y - 36);
        SDL_RenderDrawLine(r, x, y - 30, x + 14, y - 50);
    }
}

bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height) {
    if (display == NULL)
        return false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return false;
    }

    display->window = SDL_CreateWindow(title,
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width,
                                       height,
                                       SDL_WINDOW_SHOWN);

    if (display->window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    display->renderer = SDL_CreateRenderer(display->window,
                                           -1,
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);

    if (display->renderer == NULL) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(display->window);
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawBlendMode(display->renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Destroy_sdl_display(SDLDisplay *display) {
    if (display == NULL)
        return;

    if (display->renderer != NULL) {
        SDL_DestroyRenderer(display->renderer);
        display->renderer = NULL;
    }

    if (display->window != NULL) {
        SDL_DestroyWindow(display->window);
        display->window = NULL;
    }

    SDL_Quit();
}

void Clear_sdl_display(SDLDisplay *display) {
    if (display == NULL || display->renderer == NULL)
        return;

    SDL_Renderer *r = display->renderer;

    int width = 0;
    int height = 0;
    SDL_GetRendererOutputSize(r, &width, &height);

    if (width <= 0 || height <= 0)
        return;

    for (int y = 0; y < height; y += 4) {
        int blue = 38 + y * 65 / height;
        int green = 12 + y * 34 / height;

        SDL_SetRenderDrawColor(r, 4, green, blue, 255);

        SDL_Rect band = {0, y, width, 4};
        SDL_RenderFillRect(r, &band);
    }

    float time = SDL_GetTicks() / 1000.0f;

    SDL_SetRenderDrawColor(r, 5, 26, 68, 80);
    draw_filled_ellipse(r, width / 2, height / 2 + height / 6, width / 2, height / 5);

    SDL_SetRenderDrawColor(r, 3, 16, 45, 110);
    draw_filled_ellipse(r, width / 2, height + 25, width / 2, height / 4);

    int move_far = (int)(time * 15) % width;
    int move_mid = (int)(time * 28) % width;
    int move_near = (int)(time * 45) % width;

    SDL_SetRenderDrawColor(r, 40, 115, 170, 45);
    for (int y = 140; y < height; y += 170)
        draw_wave(r, width, y, move_far, 3, time * 0.7f);

    SDL_SetRenderDrawColor(r, 55, 155, 210, 60);
    for (int y = 210; y < height; y += 190)
        draw_wave(r, width, y, move_mid, 4, time * 1.0f);

    SDL_SetRenderDrawColor(r, 95, 200, 240, 35);
    for (int y = 320; y < height; y += 240)
        draw_wave(r, width, y, move_near, 5, time * 1.4f);

    SDL_SetRenderDrawColor(r, 18, 34, 78, 160);
    draw_filled_triangle(r, 0, height, 0, height - 95, 190, height);
    draw_filled_triangle(r, width, height, width, height - 125, width - 230, height);

    draw_sea_plants(r, width, height, time);
    draw_bubbles(r, width, height);
}

void Draw_fish(SDLDisplay *display, Fish *fish) {
    if (display == NULL || display->renderer == NULL || fish == NULL)
        return;

    if (fish->is_alive == false)
        return;

    SDL_Renderer *r = display->renderer;

    int x = to_int(fish->position.x);
    int y = to_int(fish->position.y);

    Vector dir = direction_or_default(fish->velocity);

    Vector side;
    side.x = -dir.y;
    side.y = dir.x;

    int size = FISH_SIZE;

    float time = SDL_GetTicks() / 1000.0f;
    float wave = sinf(time * 8.0f + x * 0.05f) * size * 0.5f;

    int base_x = to_int(x - dir.x * size * 0.6f);
    int base_y = to_int(y - dir.y * size * 0.6f);

    int q1x = to_int(base_x + side.x * size * 0.5f);
    int q1y = to_int(base_y + side.y * size * 0.5f);

    int q2x = to_int(base_x - side.x * size * 0.5f);
    int q2y = to_int(base_y - side.y * size * 0.5f);

    int q3x = to_int(x - dir.x * size * 1.6f + side.x * wave);
    int q3y = to_int(y - dir.y * size * 1.6f + side.y * wave);

    SDL_SetRenderDrawColor(r, 20, 75, 190, 255);
    draw_filled_triangle(r, q1x, q1y, q2x, q2y, q3x, q3y);

    SDL_SetRenderDrawColor(r, 40, 120, 255, 255);
    draw_filled_circle(r, x, y, size / 2);

    int nx = to_int(x + dir.x * size * 0.6f);
    int ny = to_int(y + dir.y * size * 0.6f);

    SDL_SetRenderDrawColor(r, 75, 165, 255, 255);
    draw_filled_circle(r, nx, ny, size / 4);

    int ex = to_int(x + dir.x * size / 3 - side.x * size / 5);
    int ey = to_int(y + dir.y * size / 3 - side.y * size / 5);

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    draw_filled_circle(r, ex, ey, 2);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    draw_filled_circle(r, ex, ey, 1);
}

void Draw_shark(SDLDisplay *display, Shark *shark) {
    if (display == NULL || display->renderer == NULL || shark == NULL)
        return;

    SDL_Renderer *r = display->renderer;

    int x = to_int(shark->pos.x);
    int y = to_int(shark->pos.y);

    Vector dir = direction_or_default(shark->velocity);

    Vector side;
    side.x = -dir.y;
    side.y = dir.x;

    int size = SHARK_SIZE;

    float time = SDL_GetTicks() / 1000.0f;
    float wave = sinf(time * 5.0f) * size * 0.5f;

    SDL_SetRenderDrawColor(r, 0, 0, 0, 65);
    draw_filled_ellipse(r, x, y + size / 2, size, size / 3);

    int bx = to_int(x - dir.x * size);
    int by = to_int(y - dir.y * size);

    int q1x = to_int(bx + side.x * size / 2);
    int q1y = to_int(by + side.y * size / 2);

    int q2x = to_int(bx - side.x * size / 2);
    int q2y = to_int(by - side.y * size / 2);

    int q3x = to_int(x - dir.x * size * 1.8f + side.x * wave);
    int q3y = to_int(y - dir.y * size * 1.8f + side.y * wave);

    SDL_SetRenderDrawColor(r, 80, 120, 150, 255);
    draw_filled_triangle(r, q1x, q1y, q2x, q2y, q3x, q3y);

    SDL_SetRenderDrawColor(r, 120, 160, 180, 255);
    draw_filled_ellipse(r, x, y, size, size / 2);

    int nx = to_int(x + dir.x * size * 1.4f);
    int ny = to_int(y + dir.y * size * 1.4f);

    int h1x = to_int(x + side.x * size / 2);
    int h1y = to_int(y + side.y * size / 2);

    int h2x = to_int(x - side.x * size / 2);
    int h2y = to_int(y - side.y * size / 2);

    SDL_SetRenderDrawColor(r, 120, 160, 180, 255);
    draw_filled_triangle(r, nx, ny, h1x, h1y, h2x, h2y);

    SDL_SetRenderDrawColor(r, 220, 230, 210, 230);
    draw_filled_ellipse(r,
                        to_int(x + side.x * size / 5),
                        to_int(y + side.y * size / 5),
                        size / 2,
                        size / 5);

    int ex = to_int(x + dir.x * size / 2 - side.x * size / 4);
    int ey = to_int(y + dir.y * size / 2 - side.y * size / 4);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    draw_filled_circle(r, ex, ey, 3);

    for (int i = 0; i < 3; i++) {
        int gx = to_int(x + dir.x * (size / 5 - i * 4));
        int gy = to_int(y + dir.y * (size / 5 - i * 4));

        SDL_RenderDrawLine(r,
                           gx,
                           gy,
                           to_int(gx + side.x * 10),
                           to_int(gy + side.y * 10));
    }
}

void Draw_world(SDLDisplay *display, Fish *fishes, int nb_fish, Shark *shark) {
    if (display == NULL || display->renderer == NULL)
        return;

    Clear_sdl_display(display);

    for (int i = 0; i < nb_fish; i++) {
        Draw_fish(display, &fishes[i]);
    }

    Draw_shark(display, shark);

    SDL_RenderPresent(display->renderer);
}

void Render_world(SDLDisplay *display, World *world) {
    if (display == NULL || world == NULL)
        return;

    Draw_world(display, world->fishes, world->nb_fish, world->shark);
}