#ifndef FISH_H
#define FISH_H

#include "config.h"
#include "math.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>
/**
 * @struct Fish
 * @brief Structure représentant un poisson dans le jeu.
 */
typedef struct {
    Vector position; /**< Position du poisson dans le monde */
    Vector velocity; /**< Vecteur direction/vitesse du poisson dans le monde */
    bool
        is_alive; /**< Indique si le poisson est vivant : true ou non : false */

    float radius_separation;
    float radius_alignement;
    float radius_cohesion; /**< Rayon de perception du poisson pour les règles
                              de cohésion */
    float vision_angle;    /**< Angle de vision du poisson en radians */
} Fish;

Fish Fish_create_random_pos(int width, int height);
Fish *Fish_create_random_array(int nb_fish, int width, int height);

Fish *Fish_copy_array(Fish *fish_array, int nb_fish);

void Fish_destroy_array(Fish *fish_array);

void fish_apply_action(Fish *fish, Vector action);

#endif
