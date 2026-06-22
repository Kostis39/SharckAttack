#include "render_sdl.h"
#include "collider.h"
#include "vector.h"

#include <math.h>
#include <stdio.h>

/**
 * @brief convertir float en int
 *
 * @param x valeur flottante
 * @return int valeur entiere
 */

int to_int(float x) { return (int)(x + 0.5f); }

static Vector direction_or_default(Vector v) {
    Vector d = Vector_normalize(v);

    if (d.x == 0 && d.y == 0) {
        d.x = 1;
        d.y = 0;
    }

    return d;
}

/**
 * @brief dessine un cercle plein
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre du cercle
 * @param cy coord y du centre du cercle
 * @param radius rayon du cercle
 */

void Draw_filled_circle(SDL_Renderer *r, int cx, int cy, int radius) {
    for (int y = -radius; y <= radius; y++) {
        int limit = (int)sqrtf((float)(radius * radius - y * y));
        SDL_RenderDrawLine(r, cx - limit, cy + y, cx + limit, cy + y);
    }
}

/**
 * @brief dessine une ellipse
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre de l'ellipse
 * @param cy coord y du centre de l'ellipse
 * @param rx rayon horizontal de l'ellipse
 * @param ry rayon vertical de l'ellipse
 */

void Draw_filled_ellipse(SDL_Renderer *r, int cx, int cy, int rx, int ry) {
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

/**
 * @brief dessine un triangle
 *
 * @param r renderer sdl
 * @param x1 coord x du 1er sommet
 * @param y1 coord y du 1er sommet
 * @param x2 coord x du 2eme sommet
 * @param y2 coord y du 2eme sommet
 * @param x3 coord x du 3eme sommet
 * @param y3 coord y du 3eme sommet
 */

void Draw_filled_triangle(SDL_Renderer *r, int x1, int y1, int x2, int y2,
                          int x3, int y3) {
    for (int i = 0; i <= 24; i++) {
        float t = i / 24.0f;

        int ax = to_int(x1 + (x2 - x1) * t);
        int ay = to_int(y1 + (y2 - y1) * t);

        SDL_RenderDrawLine(r, ax, ay, x3, y3);
    }
}

/**
 * @brief dessine une vague
 * la vague est obtenue avec la fct sin
 * @param r renderer sdl
 * @param width largeur de la fenetre
 * @param base_y hauteur moyenne de la vague
 * @param move decalage horizontal utilise pour l'animation
 * @param amplitude amplitude verticale de la vague
 * @param time temps courant de l'animation
 */

void Draw_wave(SDL_Renderer *r, int width, int base_y, int move, int amplitude,
               float time) {
    int old_x = -20;
    int old_y = base_y;

    for (int x = -20; x <= width + 20; x += 18) {
        int y = base_y + to_int(sinf((x + move) * 0.018f + time) * amplitude);
        SDL_RenderDrawLine(r, old_x, old_y, x, y);
        old_x = x;
        old_y = y;
    }
}

/**
 * @brief dessine des bulles
 *
 * @param r renderer sdl utilise pour dessiner
 * @param width largeur de la fenetre
 * @param height hauteur de la fenetre
 */

void Draw_bubbles(SDL_Renderer *r, int width, int height) {
    static int bx[14] = {60,  180,  310,  430,  520,  670, 790,
                         910, 1040, 1160, 1260, 1350, 250, 740};

    static int by[14] = {500, 300, 620, 180, 430, 540, 250,
                         610, 350, 470, 200, 570, 720, 80};

    if (width <= 0 || height <= 0)
        return;

    SDL_SetRenderDrawColor(r, 120, 210, 255, 90);

    for (int i = 0; i < 14; i++) {
        by[i] -= 1 + (i % 2);

        if (by[i] < -10) {
            by[i] = height + 10;
            bx[i] = (bx[i] + 173) % width;
        }

        Draw_filled_circle(r, bx[i], by[i], 2 + (i % 3));
    }
}

void Draw_mine_shape(SDL_Renderer *r, int cx, int cy, int radius) {
    int s = radius * 2;
    int p = radius / 3;

    SDL_Rect shadow = {cx - radius + 4, cy - radius + 4, s, s};
    SDL_SetRenderDrawColor(r, 0, 0, 0, 90);
    SDL_RenderFillRect(r, &shadow);

    SDL_Rect body = {cx - radius, cy - radius, s, s};
    SDL_SetRenderDrawColor(r, 45, 60, 65, 255);
    SDL_RenderFillRect(r, &body);

    SDL_SetRenderDrawColor(r, 10, 18, 22, 255);
    SDL_RenderDrawRect(r, &body);

    SDL_Rect center = {cx - p / 2, cy - p / 2, p, p};
    SDL_RenderFillRect(r, &center);

    SDL_SetRenderDrawColor(r, 95, 125, 130, 230);
    SDL_Rect light = {cx - radius + 5, cy - radius + 5, p, p};
    SDL_RenderFillRect(r, &light);
}

void Draw_demo_mines(SDLDisplay *display) {
    int width = 0;
    int height = 0;
    SDL_GetRendererOutputSize(display->renderer, &width, &height);
    if (width <= 0 || height <= 0)
        return;
    Draw_mine_shape(display->renderer, width / 4, height / 2 + 60, 12);
    Draw_mine_shape(display->renderer, width / 2, height / 2 + 60, 25);
    Draw_mine_shape(display->renderer, width - width / 4, height / 2 + 50, 12);
}

void Draw_collider(SDLDisplay *display, Collider *collider) {
    if (display == NULL || display->renderer == NULL || collider == NULL)
        return;
    int x1 = to_int(collider->bounding_box[0].x);
    int y1 = to_int(collider->bounding_box[0].y);
    int x2 = to_int(collider->bounding_box[1].x);
    int y2 = to_int(collider->bounding_box[1].y);
    int cx = (x1 + x2) / 2;
    int cy = (y1 + y2) / 2;
    int radius = (x2 - x1 < y2 - y1 ? x2 - x1 : y2 - y1) / 2;
    if (radius < 10)
        radius = 10;
    Draw_mine_shape(display->renderer, cx, cy, radius);
}

void Draw_colliders(SDLDisplay *display, Collider *colliders,
                    int nb_colliders) {
    if (display == NULL || colliders == NULL || nb_colliders <= 0)
        return;
    for (int i = 0; i < nb_colliders; i++) {
        Draw_collider(display, &colliders[i]);
    }
}

/**
 * @brief initialisation de sdl
 *
 * @param display struct contenant la fenetre et le renderer
 * @param title titre de la fenetre
 * @param width largeur
 * @param height hauteur
 * @return true si init est reussi
 * @return false
 */

bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height) {
    if (display == NULL)
        return false;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return false;
    }

    display->window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         width, height, SDL_WINDOW_SHOWN);

    if (display->window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    display->renderer = SDL_CreateRenderer(display->window, -1,
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

/**
 * @brief cette fct libere le renderer, detruit la fenetre puis ferme sdl
 *
 * @param display struct sdl a detruire
 */

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

/**
 * @brief efface l ecran et dessine le fond marin
 *
 * @param display struct contenant le renderer sdl
 */

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
    Draw_filled_ellipse(r, width / 2, height / 2 + height / 6, width / 2,
                        height / 5);

    SDL_SetRenderDrawColor(r, 3, 16, 45, 110);
    Draw_filled_ellipse(r, width / 2, height + 25, width / 2, height / 4);

    int move_far = (int)(time * 10) % width;
    int move_mid = (int)(time * 22) % width;
    int move_near = (int)(time * 45) % width;

    SDL_SetRenderDrawColor(r, 40, 115, 170, 45);
    for (int y = 140; y < height; y += 170)
        Draw_wave(r, width, y, move_far, 3, time * 0.7f);

    SDL_SetRenderDrawColor(r, 55, 155, 210, 60);
    for (int y = 210; y < height; y += 190)
        Draw_wave(r, width, y, move_mid, 4, time * 1.0f);

    SDL_SetRenderDrawColor(r, 95, 200, 240, 35);
    for (int y = 320; y < height; y += 240)
        Draw_wave(r, width, y, move_near, 5, time * 1.4f);

    SDL_SetRenderDrawColor(r, 18, 34, 78, 160);
    Draw_filled_triangle(r, 0, height, 0, height - 95, 190, height);
    Draw_filled_triangle(r, width, height, width, height - 125, width - 230,
                         height);

    /*Draw_sea_plants(r, width, height, time);*/
    Draw_bubbles(r, width, height);
}

/**
 * @brief dessine un poisson
 * le poisson est representé simplement par un corps circulaire, une petite
 * tete, une queue triangulaire animé et un oeil
 * @param display struct contenant le renderer
 * @param fish poisson à dessiner
 */

void Draw_fish(SDLDisplay *display, Fish *fish) {
    if (display == NULL || display->renderer == NULL || fish == NULL ||
        !fish->is_alive)
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
    Draw_filled_triangle(r, q1x, q1y, q2x, q2y, q3x, q3y);

    SDL_SetRenderDrawColor(r, 40, 120, 255, 255);
    Draw_filled_circle(r, x, y, size / 2);

    int nx = to_int(x + dir.x * size * 0.6f);
    int ny = to_int(y + dir.y * size * 0.6f);

    SDL_SetRenderDrawColor(r, 75, 165, 255, 255);
    Draw_filled_circle(r, nx, ny, size / 4);

    int ex = to_int(x + dir.x * size / 3 - side.x * size / 5);
    int ey = to_int(y + dir.y * size / 3 - side.y * size / 5);

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    Draw_filled_circle(r, ex, ey, 2);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    Draw_filled_circle(r, ex, ey, 1);
}

/**
 * @brief dessine le requin
 *
 * @param display struct contenant le renderer
 * @param shark requin à dessiner
 */

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
    Draw_filled_ellipse(r, x, y + size / 2, size, size / 3);

    int bx = to_int(x - dir.x * size);
    int by = to_int(y - dir.y * size);

    int q1x = to_int(bx + side.x * size / 2);
    int q1y = to_int(by + side.y * size / 2);

    int q2x = to_int(bx - side.x * size / 2);
    int q2y = to_int(by - side.y * size / 2);

    int q3x = to_int(x - dir.x * size * 1.8f + side.x * wave);
    int q3y = to_int(y - dir.y * size * 1.8f + side.y * wave);

    SDL_SetRenderDrawColor(r, 80, 120, 150, 255);
    Draw_filled_triangle(r, q1x, q1y, q2x, q2y, q3x, q3y);

    SDL_SetRenderDrawColor(r, 120, 160, 180, 255);
    Draw_filled_ellipse(r, x, y, size, size / 2);

    int nx = to_int(x + dir.x * size * 1.4f);
    int ny = to_int(y + dir.y * size * 1.4f);

    int h1x = to_int(x + side.x * size / 2);
    int h1y = to_int(y + side.y * size / 2);

    int h2x = to_int(x - side.x * size / 2);
    int h2y = to_int(y - side.y * size / 2);

    SDL_SetRenderDrawColor(r, 120, 160, 180, 255);
    Draw_filled_triangle(r, nx, ny, h1x, h1y, h2x, h2y);

    SDL_SetRenderDrawColor(r, 220, 230, 210, 230);
    Draw_filled_ellipse(r, to_int(x + side.x * size / 5),
                        to_int(y + side.y * size / 5), size / 2, size / 5);

    int ex = to_int(x + dir.x * size / 2 - side.x * size / 4);
    int ey = to_int(y + dir.y * size / 2 - side.y * size / 4);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    Draw_filled_circle(r, ex, ey, 3);

    for (int i = 0; i < 3; i++) {
        int gx = to_int(x + dir.x * (size / 5 - i * 4));
        int gy = to_int(y + dir.y * (size / 5 - i * 4));

        SDL_RenderDrawLine(r, gx, gy, to_int(gx + side.x * 10),
                           to_int(gy + side.y * 10));
    }
}

/**
 * @brief dessine le monde
 *
 * @param display struct contenant le renderer
 * @param fishes tableau contenant les poissons
 * @param nb_fish nbr de poissons dans le tableau
 * @param shark requin du monde
 */

void Draw_world(SDLDisplay *display, Fish *fishes, int nb_fish, Shark *shark) {
    if (display == NULL || display->renderer == NULL)
        return;

    Clear_sdl_display(display);

    for (int i = 0; i < nb_fish; i++) {
        Draw_fish(display, &fishes[i]);
    }

    Draw_shark(display, shark);
    // Draw_colliders(display, world->colliders, world->nb_colliders).
    Draw_demo_mines(display);
    SDL_RenderPresent(display->renderer);
}

/**
 * @brief fct principale du rendu du monde
 *
 * @param display struct contenant le renderer
 * @param world monde a afficher
 */

void Render_world(SDLDisplay *display, World *world) {
    if (display == NULL || world == NULL)
        return;

    Draw_world(display, world->fishes, world->nb_fish, world->shark);
}
