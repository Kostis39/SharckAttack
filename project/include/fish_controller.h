#ifndef AGENT_FISH_H
#define AGENT_FISH_H

#include "config.h"
#include "fish.h"
#include "vector.h"

/** @struct FishPerception
 * @brief Structure représentant la perception qu'a un poisson du monde.
 */
typedef struct {
    Fish self; /**< Le poisson lui-même, pour lequel on calcule la perception */

    Fish *neighbor_separation; /**< Tableau dynamique de poissons voisins pour
                                  la séparation */
    int nb_sep;                /**< Nombre de poissons voisins pour la
                                                  séparation */

    Fish *neighbor_alignment; /**< Tableau dynamique de poissons voisins pour
                                 l'alignement */
    int nb_align;             /**< Nombre de poissons voisins pour l'alignement
                               */

    Fish *neighbor_cohesion; /**< Tableau dynamique de poissons voisins pour la
                                cohésion */
    int nb_cohes;            /**< Nombre de poissons voisins pour la cohésion
                              */

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
Vector Rules_separation(Fish *p, int nb);
Vector Rules_alignment(Fish *p, int nb);
Vector Rules_cohesion(Fish *p, int nb);

Vector fish_choose_action(FishPerception *perception);

#endif
