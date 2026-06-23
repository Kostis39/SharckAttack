#include "render_sdl.h"
#include "sdl_draw_tools.h"
#include "collider.h"
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

    int cx = (x1 + x2) / 2;
    int cy = (y1 + y2) / 2;

    int w = x2 - x1;
    int h = y2 - y1;
    int radius = (w < h ? w : h) / 2;

    if (radius < 10)
        radius = 10;
    if (radius > 14)
        radius = 14;

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

    /* Œil */
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

    /* Œil */
    int eye_x = to_int(x + dir.x * size * 0.80f - side.x * size * 0.22f);
    int eye_y = to_int(y + dir.y * size * 0.80f - side.y * size * 0.22f);

    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    Draw_filled_circle(r, eye_x, eye_y, 3);

    /* Bouche agressive */
    SDL_RenderDrawLine(
        r, to_int(x + dir.x * size * 0.75f - side.x * size * 0.32f),
        to_int(y + dir.y * size * 0.75f - side.y * size * 0.32f),
        to_int(nose_x - dir.x * size * 0.25f - side.x * size * 0.12f),
        to_int(nose_y - dir.y * size * 0.25f - side.y * size * 0.12f));

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
    Draw_filled_triangle(r,
                         box_x + 10, 29,
                         box_x + 27, 20,
                         box_x + 27, 38);

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
 * ce mode affiche les vecteurs de vitesse, les rayons de perception.
 * il permet de comprendre visuellement le comportement multi-agents
 * @param display struct contenant le renderer
 * @param fishes tableau contenant les poissons
 * @param nb_fish nbr de poissons
 * @param shark requin a afficher en mode debug
 * @param colliders 
 * @param nb_colliders 
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
    
    /* Rectangles des colliders : même taille que la mine dessinée */
    if (colliders != NULL && nb_colliders > 0) {
        int limit = nb_colliders;

        if (limit > RENDERED_COLLIDERS)
            limit = RENDERED_COLLIDERS;

        SDL_SetRenderDrawColor(r, 255, 80, 200, 160);

        for (int i = 0; i < limit; i++) {
            int x1 = to_int(colliders[i].bounding_box[0].x);
            int y1 = to_int(colliders[i].bounding_box[0].y);
            int x2 = to_int(colliders[i].bounding_box[1].x);
            int y2 = to_int(colliders[i].bounding_box[1].y);

            int cx = (x1 + x2) / 2;
            int cy = (y1 + y2) / 2;

            int w = x2 - x1;
            int h = y2 - y1;

            if (w < 0)
                w = -w;
            if (h < 0)
                h = -h;
                
            int radius = (w < h ? w : h) / 2;

            if (radius < 10)
                radius = 10;

            if (radius > 14)
                radius = 14;

            SDL_Rect rect = {
                cx - radius,
                cy - radius,
                radius * 2,
                radius * 2
            };

            SDL_RenderDrawRect(r, &rect);

            Draw_filled_circle(r, cx - radius, cy - radius, 2);
            Draw_filled_circle(r, cx + radius, cy + radius, 2);
        }
    }
}

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

    Draw_filled_triangle(r,
                         head_x, head_y,
                         to_int(x + side.x * size * 0.4f),
                         to_int(y + side.y * size * 0.4f),
                         to_int(x - side.x * size * 0.4f),
                         to_int(y - side.y * size * 0.4f));

    SDL_RenderDrawLine(r, x, y, tail_x, tail_y);

    for (int k = 1; k <= 4; k++) {
        int bx = x + (tail_x - x) * k / 5;
        int by = y + (tail_y - y) * k / 5;

        SDL_RenderDrawLine(r, bx, by,
                           to_int(bx + side.x * 6),
                           to_int(by + side.y * 6));

        SDL_RenderDrawLine(r, bx, by,
                           to_int(bx - side.x * 6),
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

    SDL_RenderPresent(display->renderer);
}

/**
 * @brief fct principale du rendu du monde
 *
 * @param display struct contenant le renderer
 * @param world monde a afficher
 */

void Render_world(SDLDisplay *display, World *world) {
    static int old_score = 0;

    if (display == NULL || world == NULL)
        return;

    g_score = world->fish_eaten;

    if (g_score > old_score) {
        g_gain = g_score - old_score;
        g_flash = 10;
    } else if (g_flash > 0) {
        g_flash--;
    }

    old_score = g_score;

    SDL_PumpEvents();
    g_debug_view = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_V];

    for (int i = 0; i < world->nb_fish && i < FISH_NB; i++) {
        if (g_was_alive[i] && !world->fishes[i].is_alive)
            g_skeleton_frame[i] = SKELETON_FRAMES;

        g_was_alive[i] = world->fishes[i].is_alive;
    }

    Draw_world(display, world);
}