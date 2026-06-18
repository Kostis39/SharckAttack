#ifndef FISH_H
#define FISH_H

#include "config.h"
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
/**
 * @struct Fish
 * @brief Structure représentant un poisson dans le jeu.
 */
typedef struct {
    float repulsion;
    float orientation;
    float attraction;
    Vector position; /**< Position du poisson dans le monde */
    Vector velocity; /**< Vecteur direction/vitesse du poisson dans le monde */
    bool
        is_alive; /**< Indique si le poisson est vivant : true ou non : false */
    float radius; /**< Rayon de perception du poisson, utilisé pour détecter les
                 voisins et le requin */
} Fish;

Fish Fish_create_random_pos(int width, int height);
Fish *Fish_create_random_array(int nb_fish, int width, int height);

Fish *Fish_copy_array(Fish *fish_array, int nb_fish);

void Fish_destroy_array(Fish *fish_array);

#endif
