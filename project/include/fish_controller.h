#ifndef AGENT_FISH_H
#define AGENT_FISH_H

#include "fish.h"
#include "vector.h"

typedef struct FishPerception {
    Fish self; /**< Le poisson lui-même, pour lequel on calcule la perception */

    Vector *neighbor_position; /**< Tableau dynamique des positions des voisins
                                  perçus */
    Vector *neighbor_speed_vector; /**< Tableau dynamique des vecteurs vitesses
                                      des voisins perçus */
    int nb_neighbor; /**< Nombre de voisins perçus par le poisson */

    bool shark_visible; /**< Indique si le requin est visible par le poisson :
                           true ou non : false */
    Vector shark_position; /**< Position du requin dans le monde, si visible */
    Vector shark_velocity; /**< Vecteur direction/vitesse du requin dans le
                              monde, si visible */

    int width;  /**< Largeur en pixel de notre monde */
    int height; /**< Hauteur en pixel de notre monde */
} FishPerception;

/* Règles réactives individuelles : chacune transforme une FishPerc  en une
 * force désirée, sans aucun état interne ni mémoire. */
Vector Rules_separation(FishPerception *p);
Vector Rules_alignment(FishPerception *p);
Vector Rules_cohesion(FishPerception *p);

Vector fish_choose_action(FishPerception *perception);

#endif
