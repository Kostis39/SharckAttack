
#ifndef FISH_H
#define FISH_H

#include "config.h"
#include "perception.h"
#include "vector.h"
#include <stdbool.h>

typedef struct {
    Vector position;
    Vector velocity;
    bool alive; // true = vivant, false = mort
} Fish;

Fish Fish_create_random_pos(int width, int height);
// Créer un tableau de poissons avec position et vitesse aléatoire
Fish *Fish_create_random_array(int nb_fish, int width, int height);
void Fish_destroy(Fish *fish_array, int nb_fish);

/* Règles réactives individuelles : chacune transforme une FishPerception
   en une force désirée, sans aucun état interne ni mémoire. */
Vector Rules_separation(Fish *self, FishPerception *p);
Vector Rules_alignment(Fish *self, FishPerception *p);
Vector Rules_cohesion(Fish *self, FishPerception *p);

Vector fish_choose_action(Fish *fish, FishPerception *perception);

#endif