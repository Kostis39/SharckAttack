#include "sdl_draw_tools.h"
#include "config.h"
#include <math.h>

/**
 * @brief convertir float en int
 *
 * @param x valeur flottante
 * @return int valeur entiere
 */

int to_int(float x) { return (int)(x + 0.5f); }

/**
 * @brief renvoie une direction normalisée
 * transformer un vecteur vitesse en vecteur direction
 * si le vecteur est nul, elle renvoit une direction par défaut vers la droite
 * @param v vecteur a normaliser
 * @return Vector direction normalisée
 */
Vector direction_or_default(Vector v) {
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

/**
 * @brief dessine de grandes plantes
 * les plantes restent fixées au sol, seules les extrémités bougent légèrement
 * avec le temps pour donner un effet naturel sous l'eau
 * @param r renderer sdl utilisé pour dessiner
 * @param width largeur de la fenêtre
 * @param height hauteur de la fenêtre
 * @param time temps courant pour animer les plantes
 */
void Draw_sea_plants(SDL_Renderer *r, int width, int height, float time) {
    int ground = height - 6;

    for (int i = 0; i < 5; i++) {
        int x = 80 + i * ((width - 160) / 4);
        int h = 60 + (i % 2) * 18;
        int sway = to_int(sinf(time * 1.2f + i) * 6.0f);

        /* base sombre */
        SDL_SetRenderDrawColor(r, 20, 85, 95, 130);
        Draw_filled_ellipse(r, x, ground + 2, 34, 8);

        /* branches principales : vert d'eau */
        SDL_SetRenderDrawColor(r, 55, 155, 150, 195);

        SDL_RenderDrawLine(r, x, ground, x + sway, ground - h);
        SDL_RenderDrawLine(r, x, ground, x - 28 + sway, ground - h + 22);
        SDL_RenderDrawLine(r, x, ground, x + 28 + sway, ground - h + 22);
        SDL_RenderDrawLine(r, x, ground, x - 16 + sway, ground - h + 7);
        SDL_RenderDrawLine(r, x, ground, x + 16 + sway, ground - h + 7);

        /* deuxième trait très proche pour donner un peu d'épaisseur */
        SDL_RenderDrawLine(r, x + 1, ground, x + sway + 1, ground - h);
        SDL_RenderDrawLine(r, x + 1, ground, x - 27 + sway, ground - h + 22);
        SDL_RenderDrawLine(r, x + 1, ground, x + 29 + sway, ground - h + 22);
    }
}

/**
 * @brief dessine une mine carre
 * cette fct represente graphiquement un obstacle/collider
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre de la mine
 * @param cy coord y du centre de la mine
 * @param radius demi-taille de la mine
 */
void Draw_mine_shape(SDL_Renderer *r, int cx, int cy, int radius) {
    int s = radius * 2;
    int p = radius / 3;

    SDL_SetRenderDrawColor(r, 80, 180, 190, 45);
    Draw_filled_circle(r, cx, cy, radius + 10);

    SDL_Rect shadow = {cx - radius + 3, cy - radius + 3, s, s};
    SDL_Rect body = {cx - radius, cy - radius, s, s};
    SDL_Rect light = {cx - radius + 4, cy - radius + 4, p, p};
    SDL_Rect center = {cx - p / 2, cy - p / 2, p, p};

    SDL_SetRenderDrawColor(r, 0, 0, 0, 90);
    SDL_RenderFillRect(r, &shadow);

    SDL_SetRenderDrawColor(r, 45, 60, 65, 255);
    SDL_RenderFillRect(r, &body);

    SDL_SetRenderDrawColor(r, 10, 18, 22, 255);
    SDL_RenderDrawRect(r, &body);

    SDL_SetRenderDrawColor(r, 95, 125, 130, 230);
    SDL_RenderFillRect(r, &light);

    SDL_SetRenderDrawColor(r, 3, 8, 10, 255);
    SDL_RenderFillRect(r, &center);
}

/**
 * @brief dessine un chiffre avec des segments rectangulaires
 * le chiffre esr dessiné comme un afficheur a 7 segments
 * @param r renderer sdl utilisé pour dessiner
 * @param x coord x du coin sup gauche
 * @param y coord y du coin sup gauche
 * @param n chiffre a afficher entre 0 et 9
 * @param s echelle du chiffre
 */
void Draw_digit(SDL_Renderer *r, int x, int y, int n, int s) {
    int mask[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

    SDL_Rect seg[7] = {{x + s, y, 4 * s, s},
                       {x + 5 * s, y + s, s, 4 * s},
                       {x + 5 * s, y + 6 * s, s, 4 * s},
                       {x + s, y + 10 * s, 4 * s, s},
                       {x, y + 6 * s, s, 4 * s},
                       {x, y + s, s, 4 * s},
                       {x + s, y + 5 * s, 4 * s, s}};

    for (int i = 0; i < 7; i++) {
        if (mask[n] & (1 << i))
            SDL_RenderFillRect(r, &seg[i]);
    }
}

/**
 * @brief compte le nbr de chiffre d un entier (ex: 196 -> 3)
 * 
 * @param n entier dont on veut compter les chiffres
 * @return int nbr de chiffre de l entier
 */
int Count_digits(int n) {
    int digits = 1;

    while(n >= 10) {
        n = n / 10;
        digits++;
    }
    return digits;
}

/**
 * @brief dessine le contour d'un cercle
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre du cercle
 * @param cy coord y du centre du cercle
 * @param radius rayon du cercle
 */
void Draw_circle_outline(SDL_Renderer *r, int cx, int cy, int radius) {
    int old_x = cx + radius;
    int old_y = cy;

    for (int a = 10; a <= 360; a += 10) {
        float angle = a * PI / 180.0f;
        int x = cx + to_int(cosf(angle) * radius);
        int y = cy + to_int(sinf(angle) * radius);

        SDL_RenderDrawLine(r, old_x, old_y, x, y);

        old_x = x;
        old_y = y;
    }
}

/**
 * @brief dessine un vecteur orienté avec une fleche
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param pos position de depart du vecteur
 * @param dir direction du vecteur
 * @param length longueur graphique du vecteur
 */
void Draw_vector(SDL_Renderer *r, Vector pos, Vector dir, int length) {
    Vector d = direction_or_default(dir);

    Vector side;
    side.x = -d.y;
    side.y = d.x;

    int x1 = to_int(pos.x);
    int y1 = to_int(pos.y);
    int x2 = to_int(pos.x + d.x * length);
    int y2 = to_int(pos.y + d.y * length);

    SDL_RenderDrawLine(r, x1, y1, x2, y2);

    /* petite flèche */
    SDL_RenderDrawLine(r, x2, y2, to_int(x2 - d.x * 7 + side.x * 4),
                       to_int(y2 - d.y * 7 + side.y * 4));

    SDL_RenderDrawLine(r, x2, y2, to_int(x2 - d.x * 7 - side.x * 4),
                       to_int(y2 - d.y * 7 - side.y * 4));
}