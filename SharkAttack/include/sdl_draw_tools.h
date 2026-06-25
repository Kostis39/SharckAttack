#ifndef SDL_DRAW_TOOLS_H
#define SDL_DRAW_TOOLS_H

#include "vector.h"
#include "shark.h"
#include <SDL2/SDL.h>

/**
 * @brief convertir float en int
 *
 * @param x valeur flottante
 * @return int valeur entiere
 */
int to_int(float x);

/**
 * @brief renvoie une direction normalisée
 * transformer un vecteur vitesse en vecteur direction
 * si le vecteur est nul, elle renvoit une direction par défaut vers la droite
 * @param v vecteur a normaliser
 * @return Vector direction normalisée
 */
Vector direction_or_default(Vector v);

/**
 * @brief dessine un cercle plein
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre du cercle
 * @param cy coord y du centre du cercle
 * @param radius rayon du cercle
 */
void Draw_filled_circle(SDL_Renderer *r, int cx, int cy, int radius);

/**
 * @brief dessine une ellipse
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre de l'ellipse
 * @param cy coord y du centre de l'ellipse
 * @param rx rayon horizontal de l'ellipse
 * @param ry rayon vertical de l'ellipse
 */
void Draw_filled_ellipse(SDL_Renderer *r, int cx, int cy, int rx, int ry);

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
                          int x3, int y3);

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
               float time);

/**
 * @brief dessine des bulles
 *
 * @param r renderer sdl utilise pour dessiner
 * @param width largeur de la fenetre
 * @param height hauteur de la fenetre
 */
void Draw_bubbles(SDL_Renderer *r, int width, int height);

/**
 * @brief dessine de grandes plantes
 * les plantes restent fixées au sol, seules les extrémités bougent légèrement
 * avec le temps pour donner un effet naturel sous l'eau
 * @param r renderer sdl utilisé pour dessiner
 * @param width largeur de la fenêtre
 * @param height hauteur de la fenêtre
 * @param time temps courant pour animer les plantes
 */
void Draw_sea_plants(SDL_Renderer *r, int width, int height, float time);

/**
 * @brief dessine une mine carre
 * cette fct represente graphiquement un obstacle/collider
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre de la mine
 * @param cy coord y du centre de la mine
 * @param radius demi-taille de la mine
 */
void Draw_mine_shape(SDL_Renderer *r, int cx, int cy, int radius);

/**
 * @brief dessine un chiffre avec des segments rectangulaires
 * le chiffre esr dessiné comme un afficheur a 7 segments
 * @param r renderer sdl utilisé pour dessiner
 * @param x coord x du coin sup gauche
 * @param y coord y du coin sup gauche
 * @param n chiffre a afficher entre 0 et 9
 * @param s echelle du chiffre
 */
void Draw_digit(SDL_Renderer *r, int x, int y, int n, int s);

/**
 * @brief compte le nbr de chiffre d un entier (ex: 196 -> 3)
 *
 * @param n entier dont on veut compter les chiffres
 * @return int nbr de chiffre de l entier
 */
int Count_digits(int n);

/**
 * @brief dessine le contour d'un cercle
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param cx coord x du centre du cercle
 * @param cy coord y du centre du cercle
 * @param radius rayon du cercle
 */
void Draw_circle_outline(SDL_Renderer *r, int cx, int cy, int radius);

/**
 * @brief dessine un vecteur orienté avec une fleche
 *
 * @param r renderer sdl utilisé pour dessiner
 * @param pos position de depart du vecteur
 * @param dir direction du vecteur
 * @param length longueur graphique du vecteur
 */
void Draw_vector(SDL_Renderer *r, Vector pos, Vector dir, int length);

/**
 * @brief dessine l'introduction avec compte a rebours.
 *
 * La partie gauche represente le joueur.
 * La partie droite represente le bot.
 *
 * @param r renderer SDL utilise pour dessiner
 * @param left_zone zone gauche de l'ecran
 * @param right_zone zone droite de l'ecran
 * @param number nombre affiche pendant le compte a rebours
 * @param left_shark requin gauche
 * @param right_shark requin droit
 */
void Draw_intro_countdown(SDL_Renderer *r, SDL_Rect left_zone,
                          SDL_Rect right_zone, int number,
                          Shark *left_shark, Shark *right_shark);

#endif