
#ifndef FISH_H
#define FISH_H

#include "config.h"
#include "vector.h"
#include <stdbool.h>

typedef struct {
    Vector position;
    Vector speed_vector;
    bool alive; // true = vivant, false = mort
} Fish;

Fish *FishCreate(Vector pos); // Créer un poisson avec une position en parmètre
                              // et une vitesse aléatoire

#endif