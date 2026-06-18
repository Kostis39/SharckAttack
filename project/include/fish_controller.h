#ifndef AGENT_FISH_H
#define AGENT_FISH_H

#include "fish.h"

typedef struct fishPerception {
    Fish self;

    Vector *neighbor_position;
    Vector *neighbor_speed_vector;
    int nb_neighbor; // nombre de voisins perçus

    bool shark_visible; // true si le requin est visible par le possion
    Vector shark_position;
    Vector shark_velocity;

    int width;
    int height;
} FishPerception;

/* Règles réactives individuelles : chacune transforme une FishPerception
   en une force désirée, sans aucun état interne ni mémoire. */
Vector Rules_separation(Fish *self, FishPerception *p);
Vector Rules_alignment(Fish *self, FishPerception *p);
Vector Rules_cohesion(Fish *self, FishPerception *p);

Vector fish_choose_action(Fish *fish, FishPerception *perception);

#endif
