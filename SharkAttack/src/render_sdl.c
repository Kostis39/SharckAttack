#include "render_sdl.h"
#include "collider.h"
#include "sdl_draw_tools.h"
#include "world.h"
#include <math.h>
#include <stdio.h>

#define RENDERED_COLLIDERS 5
#define SKELETON_FRAMES 45

int g_score = 0;
int g_gain = 0;
int g_flash = 0;
int g_debug_view = 0;
int g_was_alive[FISH_NB] = {0};
int g_skeleton_frame[FISH_NB] = {0};

/**
 * @brief dessine un collider sous forme de mine
 *
 * @param display struct contenant le renderer sdl
 * @param collider collider a afficher
 */
void Draw_collider(SDLDisplay *display, Collider *collider) {
    if (display == NULL || display->renderer == NULL || collider == NULL)
        return;

    int x1 = to_int(collider->bounding_box[0].x);
    int y1 = to_int(collider->bounding_box[0].y);
    int x2 = to_int(collider->bounding_box[1].x);
    int y2 = to_int(collider->bounding_box[1].y);

    if (x2 < x1) {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if (y2 < y1) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    int cx = (x1 + x2) / 2;
    int cy = (y1 + y2) / 2;

    int w = x2 - x1;
    int h = y2 - y1;

    int radius = (w < h ? w : h) / 2;

    Draw_mine_shape(display->renderer, cx, cy, radius);
}

/**
 * @brief dessine un tableau de colliders
 *
 * @param display struct contenant le renderer sdl
 * @param colliders tableau de colliders a afficher
 * @param nb_colliders nbr de colliders dans le tableau
 */
void Draw_colliders(SDLDisplay *display, Collider *colliders,
                    int nb_colliders) {
    if (display == NULL || colliders == NULL || nb_colliders <= 0)
        return;

    if (nb_colliders > RENDERED_COLLIDERS)
        nb_colliders = RENDERED_COLLIDERS;

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

    display->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, width, height,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (display->window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    display->renderer = SDL_CreateRenderer(display->window, -1,
                                           SDL_RENDERER_ACCELERATED |
                                               SDL_RENDERER_PRESENTVSYNC |
                                               SDL_RENDERER_TARGETTEXTURE);

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

    if (display->left_scene != NULL) {
        SDL_DestroyTexture(display->left_scene);
        display->left_scene = NULL;
    }

    if (display->right_scene != NULL) {
        SDL_DestroyTexture(display->right_scene);
        display->right_scene = NULL;
    }

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

    Draw_sea_plants(r, width, height, time);
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
    if (display == NULL || display->renderer == NULL || fish == NULL)
        return;

    if (fish->is_alive == false)
        return;

    SDL_Renderer *r = display->renderer;

    int x = to_int(fish->position.x);
    int y = to_int(fish->position.y);
    int size = FISH_SIZE;

    Vector dir = direction_or_default(fish->velocity);

    Vector side;
    side.x = -dir.y;
    side.y = dir.x;

    float time = SDL_GetTicks() / 1000.0f;
    float move_tail = sinf(time * 8.0f + x * 0.05f) * size * 0.45f;

    int head_x = to_int(x + dir.x * size * 0.45f);
    int head_y = to_int(y + dir.y * size * 0.45f);

    int tail_base_x = to_int(x - dir.x * size * 0.55f);
    int tail_base_y = to_int(y - dir.y * size * 0.55f);

    int tail_tip_x = to_int(x - dir.x * size * 1.45f + side.x * move_tail);
    int tail_tip_y = to_int(y - dir.y * size * 1.45f + side.y * move_tail);

    /* Queue */
    SDL_SetRenderDrawColor(r, 20, 75, 190, 255);
    Draw_filled_triangle(r, to_int(tail_base_x + side.x * size * 0.45f),
                         to_int(tail_base_y + side.y * size * 0.45f),
                         to_int(tail_base_x - side.x * size * 0.45f),
                         to_int(tail_base_y - side.y * size * 0.45f),
                         tail_tip_x, tail_tip_y);

    /* Corps */
    SDL_SetRenderDrawColor(r, 40, 120, 255, 255);
    Draw_filled_circle(r, x, y, size / 2);

    /* Petite tête claire */
    SDL_SetRenderDrawColor(r, 85, 175, 255, 255);
    Draw_filled_circle(r, head_x, head_y, size / 4);

    /* Petite nageoire */
    SDL_SetRenderDrawColor(r, 15, 65, 170, 230);
    Draw_filled_triangle(
        r, x, y, to_int(x - dir.x * size * 0.2f + side.x * size * 0.35f),
        to_int(y - dir.y * size * 0.2f + side.y * size * 0.35f),
        to_int(x + side.x * size * 0.75f), to_int(y + side.y * size * 0.75f));

    /* Oeil */
    int eye_x = to_int(x + dir.x * size * 0.35f - side.x * size * 0.18f);
    int eye_y = to_int(y + dir.y * size * 0.35f - side.y * size * 0.18f);

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    Draw_filled_circle(r, eye_x, eye_y, 2);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    Draw_filled_circle(r, eye_x, eye_y, 1);
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
    int size = SHARK_SIZE;

    Vector dir = direction_or_default(shark->velocity);

    Vector side;
    side.x = -dir.y;
    side.y = dir.x;

    float time = SDL_GetTicks() / 1000.0f;
    float move_tail = sinf(time * 5.0f) * size * 0.35f;

    int nose_x = to_int(x + dir.x * size * 1.45f);
    int nose_y = to_int(y + dir.y * size * 1.45f);
    if (g_flash > 0) {
        int a = 12 - g_flash;

        SDL_SetRenderDrawColor(r, 210, 20, 25, 180);

        Draw_filled_circle(r, to_int(nose_x + dir.x * a),
                           to_int(nose_y + dir.y * a), 2);

        Draw_filled_circle(r, to_int(nose_x + side.x * 5 + dir.x * a),
                           to_int(nose_y + side.y * 5 + dir.y * a), 2);

        Draw_filled_circle(r, to_int(nose_x - side.x * 5 + dir.x * a),
                           to_int(nose_y - side.y * 5 + dir.y * a), 2);

        SDL_SetRenderDrawColor(r, 255, 150, 50, 170);

        SDL_RenderDrawLine(r, nose_x, nose_y, to_int(nose_x + dir.x * (a + 8)),
                           to_int(nose_y + dir.y * (a + 8)));
    }

    int tail_x = to_int(x - dir.x * size * 1.05f);
    int tail_y = to_int(y - dir.y * size * 1.05f);

    int top_x = to_int(x + side.x * size * 0.55f);
    int top_y = to_int(y + side.y * size * 0.55f);

    int bottom_x = to_int(x - side.x * size * 0.55f);
    int bottom_y = to_int(y - side.y * size * 0.55f);

    /* Ombre */
    SDL_SetRenderDrawColor(r, 0, 0, 0, 70);
    Draw_filled_ellipse(r, x, y + size / 2, size + 5, size / 3);

    /* Corps principal : deux triangles orientés */
    SDL_SetRenderDrawColor(r, 95, 135, 155, 255);
    Draw_filled_triangle(r, nose_x, nose_y, top_x, top_y, tail_x, tail_y);
    Draw_filled_triangle(r, nose_x, nose_y, bottom_x, bottom_y, tail_x, tail_y);

    /* Dos sombre */
    SDL_SetRenderDrawColor(r, 45, 75, 95, 240);
    Draw_filled_triangle(r, nose_x, nose_y, top_x, top_y,
                         to_int(x - dir.x * size * 0.7f),
                         to_int(y - dir.y * size * 0.7f));

    /* Ventre clair */
    SDL_SetRenderDrawColor(r, 225, 235, 215, 240);
    Draw_filled_triangle(r, nose_x, nose_y, bottom_x, bottom_y,
                         to_int(x - dir.x * size * 0.65f),
                         to_int(y - dir.y * size * 0.65f));

    /* Grande nageoire dorsale */
    SDL_SetRenderDrawColor(r, 45, 75, 95, 255);
    Draw_filled_triangle(
        r, to_int(x - dir.x * size * 0.25f + side.x * size * 0.45f),
        to_int(y - dir.y * size * 0.25f + side.y * size * 0.45f),
        to_int(x + dir.x * size * 0.25f + side.x * size * 0.40f),
        to_int(y + dir.y * size * 0.25f + side.y * size * 0.40f),
        to_int(x + side.x * size * 1.20f), to_int(y + side.y * size * 1.20f));

    /* Queue, pointe attachée au corps */
    int back_x = to_int(tail_x - dir.x * size * 0.75f + side.x * move_tail);
    int back_y = to_int(tail_y - dir.y * size * 0.75f + side.y * move_tail);

    int tail_top_x = to_int(back_x + side.x * size * 0.40f);
    int tail_top_y = to_int(back_y + side.y * size * 0.40f);

    int tail_bot_x = to_int(back_x - side.x * size * 0.40f);
    int tail_bot_y = to_int(back_y - side.y * size * 0.40f);

    SDL_SetRenderDrawColor(r, 60, 100, 125, 255);

    Draw_filled_triangle(r, tail_x, tail_y, tail_top_x, tail_top_y, back_x,
                         back_y);

    Draw_filled_triangle(r, tail_x, tail_y, tail_bot_x, tail_bot_y, back_x,
                         back_y);

    /* Oeil */
    int eye_x = to_int(x + dir.x * size * 0.65f + side.x * size * 0.22f);
    int eye_y = to_int(y + dir.y * size * 0.65f + side.y * size * 0.22f);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    Draw_filled_circle(r, eye_x, eye_y, 2);

    /* Bouche */
    int mouth_x1 = to_int(x + dir.x * size * 0.75f - side.x * size * 0.32f);
    int mouth_y1 = to_int(y + dir.y * size * 0.75f - side.y * size * 0.32f);
    int mouth_x2 =
        to_int(nose_x - dir.x * size * 0.25f - side.x * size * 0.12f);
    int mouth_y2 =
        to_int(nose_y - dir.y * size * 0.25f - side.y * size * 0.12f);

    for (int i = 0; i < 3; i++) {
        SDL_RenderDrawLine(r, mouth_x1, mouth_y1 + i, mouth_x2, mouth_y2 + i);
    }

    /* Branchies */
    for (int i = 0; i < 3; i++) {
        int gx = to_int(x + dir.x * (size * 0.25f - i * 4));
        int gy = to_int(y + dir.y * (size * 0.25f - i * 4));

        SDL_RenderDrawLine(r, gx, gy, to_int(gx - side.x * size * 0.35f),
                           to_int(gy - side.y * size * 0.35f));
    }
}

/**
 * @brief dessine le score en haut a droite de l ecran
 * le score correspond au nbr de poissons mangés par le requin
 * @param display struct contenant le renderer
 */
void Draw_score(SDLDisplay *display) {
    SDL_Renderer *r = display->renderer;
    int w = 0, h = 0;

    SDL_GetRendererOutputSize(r, &w, &h);

    if (w <= 0 || h <= 0)
        return;

    int digits = Count_digits(g_score);

    int digit_width = 17;
    int box_w = 50 + digits * digit_width;
    int box_x = w - box_w - 17;
    int start_x = box_x + 38;

    SDL_Rect box = {box_x, 12, box_w, 34};

    SDL_SetRenderDrawColor(r, 0, 15, 30, 180);
    SDL_RenderFillRect(r, &box);

    SDL_SetRenderDrawColor(r, 80, 190, 230, 160);
    SDL_RenderDrawRect(r, &box);

    /* icône requin : elle recule automatiquement avec la boîte */
    SDL_SetRenderDrawColor(r, 120, 170, 190, 255);
    Draw_filled_triangle(r, box_x + 10, 29, box_x + 27, 20, box_x + 27, 38);

    SDL_SetRenderDrawColor(r, 180, 240, 255, 255);

    int value = g_score;

    for (int i = digits - 1; i >= 0; i--) {
        Draw_digit(r, start_x + i * digit_width, 19, value % 10, 2);
        value = value / 10;
    }

    if (g_flash > 0) {
        SDL_SetRenderDrawColor(r, 255, 220, 80, 255);

        SDL_RenderDrawLine(r, box_x + 35, 53, box_x + 45, 53);
        SDL_RenderDrawLine(r, box_x + 40, 48, box_x + 40, 58);

        Draw_digit(r, box_x + 52, 45, g_gain % 10, 1);
    }
}

/**
 * @brief dessine le mode de visualisation debug
 * ce mode affiche les vecteurs de vitesse, les rayons de perception
 * il permet de comprendre visuellement le comportement multi-agents
 * @param display struct contenant le renderer
 * @param fishes tableau contenant les poissons
 * @param nb_fish nbr de poissons
 * @param shark requin a afficher en mode debug
 * @param colliders tableau contenant les colliders du monde
 * @param nb_colliders nbr de colliders dans le tableau
 */
void Draw_debug_view(SDLDisplay *display, Fish *fishes, int nb_fish,
                     Shark *shark, Collider *colliders, int nb_colliders) {
    SDL_Renderer *r = display->renderer;

    if (fishes == NULL || shark == NULL)
        return;

    /* Vecteurs des poissons */
    SDL_SetRenderDrawColor(r, 120, 230, 255, 160);

    for (int i = 0; i < nb_fish; i++) {
        if (fishes[i].is_alive) {
            Draw_vector(r, fishes[i].position, fishes[i].velocity, 22);
        }
    }

    /* 3 cercles boids : cohésion, alignement, séparation */
    for (int i = 0; i < nb_fish; i += 30) {
        if (fishes[i].is_alive) {
            int x = to_int(fishes[i].position.x);
            int y = to_int(fishes[i].position.y);

            /* Grand cercle : cohésion */
            SDL_SetRenderDrawColor(r, 80, 180, 255, 70);
            Draw_circle_outline(r, x, y, to_int(fishes[i].radius_cohesion));

            /* Cercle moyen : alignement */
            SDL_SetRenderDrawColor(r, 255, 200, 80, 90);
            Draw_circle_outline(r, x, y, to_int(fishes[i].radius_alignement));

            /* Petit cercle : séparation */
            SDL_SetRenderDrawColor(r, 255, 80, 80, 130);
            Draw_circle_outline(r, x, y, to_int(fishes[i].radius_separation));
        }
    }

    /* Vecteur du requin */
    SDL_SetRenderDrawColor(r, 255, 220, 80, 220);
    Draw_vector(r, shark->pos, shark->velocity, 45);

    /* Vision du requin */
    SDL_SetRenderDrawColor(r, 255, 80, 80, 80);
    Draw_circle_outline(r, to_int(shark->pos.x), to_int(shark->pos.y),
                        SHARK_VISION_RANGE);

    /* Zone d'attaque du requin */
    SDL_SetRenderDrawColor(r, 255, 60, 40, 160);
    Draw_circle_outline(r, to_int(shark->pos.x), to_int(shark->pos.y),
                        SHARK_ATTACK_RANGE);

    /* Rectangle réel des colliders */
    if (colliders != NULL && nb_colliders > 0) {
        int limit = nb_colliders;

        if (limit > RENDERED_COLLIDERS)
            limit = RENDERED_COLLIDERS;

        SDL_SetRenderDrawColor(r, 255, 80, 200, 170);

        for (int i = 0; i < limit; i++) {
            int x1 = to_int(colliders[i].bounding_box[0].x);
            int y1 = to_int(colliders[i].bounding_box[0].y);
            int x2 = to_int(colliders[i].bounding_box[1].x);
            int y2 = to_int(colliders[i].bounding_box[1].y);

            if (x2 < x1) {
                int tmp = x1;
                x1 = x2;
                x2 = tmp;
            }

            if (y2 < y1) {
                int tmp = y1;
                y1 = y2;
                y2 = tmp;
            }

            SDL_Rect rect = {x1, y1, x2 - x1, y2 - y1};

            SDL_RenderDrawRect(r, &rect);

            Draw_filled_circle(r, x1, y1, 2);
            Draw_filled_circle(r, x2, y2, 2);
        }
    }
}

/**
 * @brief dessine le squelette d'un poisson mangé
 * le squelette est affiché pendant qlq frames après la mort d un poisson
 * @param display struct contenant le renderer sdl
 * @param fish poisson dont on veut afficher le squelette
 * @param frame num de frame restant pour l animation du squelette
 */
void Draw_fish_skeleton(SDLDisplay *display, Fish *fish, int frame) {
    SDL_Renderer *r = display->renderer;
    Vector dir = direction_or_default(fish->velocity);
    Vector side = {-dir.y, dir.x};

    int x = to_int(fish->position.x);
    int y = to_int(fish->position.y + (SKELETON_FRAMES - frame) / 2);
    int size = FISH_SIZE;

    SDL_SetRenderDrawColor(r, 220, 240, 230, 170);

    int head_x = to_int(x + dir.x * size * 0.6f);
    int head_y = to_int(y + dir.y * size * 0.6f);
    int tail_x = to_int(x - dir.x * size * 1.4f);
    int tail_y = to_int(y - dir.y * size * 1.4f);

    Draw_filled_triangle(r, head_x, head_y, to_int(x + side.x * size * 0.4f),
                         to_int(y + side.y * size * 0.4f),
                         to_int(x - side.x * size * 0.4f),
                         to_int(y - side.y * size * 0.4f));

    SDL_RenderDrawLine(r, x, y, tail_x, tail_y);

    for (int k = 1; k <= 4; k++) {
        int bx = x + (tail_x - x) * k / 5;
        int by = y + (tail_y - y) * k / 5;

        SDL_RenderDrawLine(r, bx, by, to_int(bx + side.x * 6),
                           to_int(by + side.y * 6));

        SDL_RenderDrawLine(r, bx, by, to_int(bx - side.x * 6),
                           to_int(by - side.y * 6));
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

void Draw_world(SDLDisplay *display, World *world) {
    if (display == NULL || display->renderer == NULL || world == NULL)
        return;

    Clear_sdl_display(display);

    for (int i = 0; i < world->nb_fish; i++) {
        Draw_fish(display, &world->fishes[i]);
    }
    for (int i = 0; i < world->nb_fish && i < FISH_NB; i++) {
        if (g_skeleton_frame[i] > 0) {
            Draw_fish_skeleton(display, &world->fishes[i], g_skeleton_frame[i]);
            g_skeleton_frame[i]--;
        }
    }

    Draw_colliders(display, world->colliders, world->nb_colliders);

    // int width = 0;
    // int height = 0;
    /* int radius = 12; */
    /* int margin = 4; */

    // SDL_GetRendererOutputSize(display->renderer, &width, &height);

    /* if (width > 0 && height > 0) { */
    /*     Draw_mine_shape(display->renderer, margin + radius, margin + radius,
     */
    /*                     radius); */
    /*     Draw_mine_shape(display->renderer, width - margin - radius, */
    /*                     height - margin - radius, radius); */
    /* } */

    Draw_shark(display, world->shark);

    if (g_debug_view)
        Draw_debug_view(display, world->fishes, world->nb_fish, world->shark,
                        world->colliders, world->nb_colliders);

    Draw_score(display);
}

/**
 * @brief dessine un nombre avec les chiffres deja existants.
 *
 * @param r renderer SDL
 * @param x position x
 * @param y position y
 * @param value nombre a afficher
 */
void Draw_result_number(SDL_Renderer *r, int x, int y, int value) {
    int digits = Count_digits(value);

    for (int i = digits - 1; i >= 0; i--) {
        Draw_digit(r, x + i * 17, y, value % 10, 2);
        value = value / 10;
    }
}

/**
 * @brief dessine un grand W ou un grand L avec des lignes.
 *
 * @param r renderer SDL
 * @param x position x
 * @param y position y
 * @param is_winner 1 pour W, 0 pour L
 */
void Draw_big_result_letter(SDL_Renderer *r, int x, int y,
                                   int is_winner) {
    int h = 70;

    if (is_winner) {
        /* W */
        SDL_RenderDrawLine(r, x, y, x + 15, y + h);
        SDL_RenderDrawLine(r, x + 15, y + h, x + 35, y + 35);
        SDL_RenderDrawLine(r, x + 35, y + 35, x + 55, y + h);
        SDL_RenderDrawLine(r, x + 55, y + h, x + 70, y);
    } else {
        /* L */
        SDL_RenderDrawLine(r, x, y, x, y + h);
        SDL_RenderDrawLine(r, x, y + h, x + 65, y + h);
    }
}

/**
 * @brief affiche le resultat sans cacher le jeu.
 *
 * Cette fonction colore tout le fond de la moitie avec un filtre transparent :
 * vert pour winner, rouge pour loser. Le jeu reste visible derriere.
 *
 * @param r renderer SDL
 * @param zone moitie gauche ou droite
 * @param is_winner 1 = winner, 0 = loser
 * @param time_ms temps final de cette partie
 * @param diff_ms difference de temps entre les deux parties
 * @param show_diff 1 si on affiche la difference
 */
void Draw_result_overlay(SDL_Renderer *r, SDL_Rect zone, int is_winner,
                                int time_ms, int diff_ms, int show_diff) {
    /*
     * Filtre transparent sur tout le fond du jeu.
     * Alpha faible : on voit encore le requin et le monde.
     */
    if (is_winner)
        SDL_SetRenderDrawColor(r, 20, 220, 100, 55);
    else
        SDL_SetRenderDrawColor(r, 220, 30, 50, 55);

    SDL_RenderFillRect(r, &zone);

    /*
     * Bordure de la moitie.
     */
    if (is_winner)
        SDL_SetRenderDrawColor(r, 80, 255, 150, 230);
    else
        SDL_SetRenderDrawColor(r, 255, 90, 100, 230);

    SDL_RenderDrawRect(r, &zone);

    /*
     * Lettre W ou L au centre.
     */
    if (is_winner)
        SDL_SetRenderDrawColor(r, 140, 255, 190, 255);
    else
        SDL_SetRenderDrawColor(r, 255, 130, 140, 255);

    Draw_big_result_letter(r,
                           zone.x + zone.w / 2 - 35,
                           zone.y + zone.h / 2 - 35,
                           is_winner);

    /*
     * Temps final en secondes en bas a gauche.
     */
    SDL_SetRenderDrawColor(r, 220, 240, 255, 255);
    Draw_result_number(r, zone.x + 25, zone.y + zone.h - 45,
                       time_ms / 1000);

    /*
     * Difference de temps en bas a droite.
     * Elle s'affiche seulement quand les deux ont fini.
     */
    if (show_diff) {
        SDL_SetRenderDrawColor(r, 255, 220, 80, 255);

        int x = zone.x + zone.w - 95;
        int y = zone.y + zone.h - 35;

        /* petit + */
        SDL_RenderDrawLine(r, x, y, x + 15, y);
        SDL_RenderDrawLine(r, x + 7, y - 7, x + 7, y + 7);

        Draw_result_number(r, x + 25, zone.y + zone.h - 45,
                           diff_ms / 1000);
    }
}

/**
 * @brief affiche deux vues dans une seule fenêtre.
 *
 * Le premier monde est affiché à gauche.
 * Le deuxième monde est affiché à droite.
 *
 * pour l'instant, Render_world appelle cette fonction avec le même monde
 * deux fois? plus tard, appeler directement :
 * Render_two_worlds(display, world_bot, world_user);
 *
 * @param display structure SDL contenant la fenêtre et le renderer
 * @param left_world monde affiché à gauche
 * @param right_world monde affiché à droite
 */
void Render_two_worlds(SDLDisplay *display, World *left_world,World *right_world) {
    static int texture_w = 0;
    static int texture_h = 0;
    static int window_was_doubled = 0;

    static int old_score[2] = {0, 0};
    static int gain[2] = {0, 0};
    static int flash[2] = {0, 0};
    static int was_alive[2][FISH_NB] = {{0}};
    static int skeleton_frame[2][FISH_NB] = {{0}};

    static int start_time = 0;
    static int finish_time[2] = {-1, -1};
    static int finished[2] = {0, 0};
    static int winner = -1;

    if (display == NULL || display->renderer == NULL || left_world == NULL)
        return;

    if (right_world == NULL)
        right_world = left_world;

    SDL_Renderer *r = display->renderer;

    int world_w = left_world->width;
    int world_h = left_world->height;

    if (world_w <= 0 || world_h <= 0)
        return;

    /*
     * On agrandit la fenêtre une seule fois.
     * Comme ça, chaque moitié garde une taille propre.
     */
    if (!window_was_doubled) {
        SDL_SetWindowSize(display->window, world_w * 2 * 0.85f, world_h);
        window_was_doubled = 1;
    }

    /*
     * Création des textures où on dessine chaque vue.
     */
    if (display->left_scene == NULL || texture_w != world_w || texture_h != world_h) {
        if (display->left_scene != NULL)
            SDL_DestroyTexture(display->left_scene);

        if (display->right_scene != NULL)
            SDL_DestroyTexture(display->right_scene);

        display->left_scene =
            SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, world_w, world_h);

        display->right_scene =
            SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, world_w, world_h);

        texture_w = world_w;
        texture_h = world_h;
    }

    if (display->left_scene == NULL || display->right_scene == NULL)
        return;

    SDL_PumpEvents();
    g_debug_view = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_V];

    World *worlds[2] = {left_world, right_world};
    SDL_Texture *scenes[2] = {display->left_scene, display->right_scene};

    int nb_views = 2;

    /*
     * Si les deux côtés utilisent le même world, on dessine une seule fois,
     * puis on copie la même image deux fois.
     */
    if (left_world == right_world)
        nb_views = 1;

    for (int p = 0; p < nb_views; p++) {
        World *world = worlds[p];

        g_score = world->fish_eaten;

        if (g_score > old_score[p]) {
            gain[p] = g_score - old_score[p];
            flash[p] = 10;
        } else if (flash[p] > 0) {
            flash[p]--;
        }

        old_score[p] = g_score;

        g_gain = gain[p];
        g_flash = flash[p];

        for (int i = 0; i < world->nb_fish && i < FISH_NB; i++) {
            if (was_alive[p][i] && !world->fishes[i].is_alive)
                skeleton_frame[p][i] = SKELETON_FRAMES;

            was_alive[p][i] = world->fishes[i].is_alive;
            g_skeleton_frame[i] = skeleton_frame[p][i];
        }

        /*
         * On dessine la vue dans une texture, pas directement dans la fenêtre.
         */
        SDL_SetRenderTarget(r, scenes[p]);

        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
        SDL_RenderClear(r);

        Draw_world(display, world);

        for (int i = 0; i < world->nb_fish && i < FISH_NB; i++) {
            skeleton_frame[p][i] = g_skeleton_frame[i];
        }
    }

    /*
     * On revient à la vraie fenêtre.
     */
    SDL_SetRenderTarget(r, NULL);

    int window_w = 0;
    int window_h = 0;
    SDL_GetRendererOutputSize(r, &window_w, &window_h);

    if (window_w <= 0 || window_h <= 0)
        return;

    SDL_SetRenderDrawColor(r, 2, 8, 20, 255);
    SDL_RenderClear(r);

    SDL_Rect left_screen = {0, 0, window_w / 2, window_h};
    SDL_Rect right_screen = {window_w / 2, 0, window_w - window_w / 2,
                             window_h};

    /*
     * Partie gauche.
     */
    SDL_RenderCopy(r, display->left_scene, NULL, &left_screen);

    /*
     * Partie droite.
     * Si c'est le même world, on recopie left_scene.
     * Sinon, on affiche right_scene.
     */
    if (left_world == right_world)
        SDL_RenderCopy(r, display->left_scene, NULL, &right_screen);
    else
        SDL_RenderCopy(r, display->right_scene, NULL, &right_screen);

    /*
     * Séparation au milieu.
     */
    SDL_SetRenderDrawColor(r, 10, 25, 50, 255);
    SDL_RenderDrawLine(r, window_w / 2 - 2, 0, window_w / 2 - 2, window_h);
    SDL_RenderDrawLine(r, window_w / 2 + 2, 0, window_w / 2 + 2, window_h);

    SDL_SetRenderDrawColor(r, 120, 220, 255, 220);
    SDL_RenderDrawLine(r, window_w / 2, 0, window_w / 2, window_h);

    /*
     * Cadres autour des deux écrans.
     */
    SDL_SetRenderDrawColor(r, 90, 180, 220, 180);
    SDL_RenderDrawRect(r, &left_screen);
    SDL_RenderDrawRect(r, &right_screen);

    /*
    * Gestion de fin :
    * le premier cote qui mange tous ses poissons devient winner.
    * L'autre continue a jouer. Il devient loser seulement quand il finit aussi.
    */
    int now = (int)SDL_GetTicks();

    if (start_time == 0)
        start_time = now;

    int elapsed = now - start_time;

    /*
    * Reset simple si une nouvelle partie commence.
    */
    if (left_world->fish_eaten == 0 && right_world->fish_eaten == 0 &&
        (winner != -1 || finished[0] || finished[1])) {
        start_time = now;

        finish_time[0] = -1;
        finish_time[1] = -1;

        finished[0] = 0;
        finished[1] = 0;

        winner = -1;
    }

    /*
    * Cote gauche fini.
    */
    if (!finished[0] && left_world->fish_eaten >= left_world->nb_fish) {
        finished[0] = 1;
        finish_time[0] = elapsed;

        if (winner == -1)
            winner = 0;
    }

    /*
    * Cote droit fini.
    */
    if (!finished[1] && right_world->fish_eaten >= right_world->nb_fish) {
        finished[1] = 1;
        finish_time[1] = elapsed;

        if (winner == -1)
            winner = 1;
    }

    /*
    * Cas de test : si c'est le meme monde affiche deux fois,
    * on met W dans les deux ecrans.
    */
    if (left_world == right_world && finished[0]) {
        Draw_result_overlay(r, left_screen, 1, finish_time[0], 0, 0);
        Draw_result_overlay(r, right_screen, 1, finish_time[0], 0, 0);
    } else if (winner != -1) {
        int diff = 0;
        int show_diff = finished[0] && finished[1];

        if (show_diff) {
            diff = finish_time[0] - finish_time[1];

            if (diff < 0)
                diff = -diff;
        }

        /*
        * Si la gauche a gagne.
        */
        if (winner == 0) {
            Draw_result_overlay(r, left_screen, 1,
                                finish_time[0], diff, show_diff);

            /*
            * La droite devient loser seulement quand elle finit aussi.
            */
            if (finished[1])
                Draw_result_overlay(r, right_screen, 0,
                                    finish_time[1], diff, show_diff);
        }

        /*
        * Si la droite a gagne.
        */
        if (winner == 1) {
            Draw_result_overlay(r, right_screen, 1,
                                finish_time[1], diff, show_diff);

            /*
            * La gauche devient loser seulement quand elle finit aussi.
            */
            if (finished[0])
                Draw_result_overlay(r, left_screen, 0,
                                    finish_time[0], diff, show_diff);
        }
    }

    SDL_RenderPresent(r);
}

/**
 * @brief rendu principal actuel.
 * NOTE: ne pas utiliser fonction de démo.
 * @param display struct sdl
 * @param world monde à afficher
 */
void Render_world(SDLDisplay *display, World *world) {
    Render_two_worlds(display, world, world);
}