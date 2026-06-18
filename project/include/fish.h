#ifndef FISH_H
#define FISH_H

#include "config.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    Vector position;
    Vector velocity;
    bool alive; // true = vivant, false = mort
} Fish;

Fish Fish_create_random_pos(int width, int height);
// Créer un tableau de poissons avec position et vitesse aléatoire
Fish *Fish_create_random_array(int nb_fish, int width, int height);
void Fish_destroy_array(Fish *fish_array);

#endif
