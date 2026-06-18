#ifndef FISH_H
#define FISH_H

#include "vector.h"

struct FishPerception;

typedef struct {
    Vector position;
    Vector velocity;
    bool alive; // true = vivant, false = mort
} Fish;

Fish *Fish_create(Vector pos); // Créer un poisson avec une position en parmètre
                               // et une vitesse aléatoire

/* Règles réactives individuelles : chacune transforme une FishPerception
   en une force désirée, sans aucun état interne ni mémoire. */
Vector Rules_separation(Fish *self, struct FishPerception *p);
Vector Rules_alignment(Fish *self, struct FishPerception *p);
Vector Rules_cohesion(Fish *self, struct FishPerception *p);

Vector fish_choose_action(Fish *fish, struct FishPerception *perception);

#endif
