#ifndef PERCEPTION_H
#define PERCEPTION_H

#include "vector.h"
#include "fish.h"

struct World;

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

FishPerception *get_fish_perception(Fish *, struct World *);
// SharkPerception* get_shark_perception(World*);

#endif
