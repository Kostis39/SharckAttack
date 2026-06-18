#ifndef PERCEPTION_H
#define PERCEPTION_H

#include "fish.h"
#include "vector.h"

typedef struct {
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

FishPerception *get_fish_perception(Fish, World *);
// SharkPerception* get_shark_perception(World*);

#endif
