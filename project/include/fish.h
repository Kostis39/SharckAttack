
#ifndef FISH_H
#define FISH_H

#include "vector.h"

typedef struct {
    Vec2 position;
    Vec2 speed_vector;
    float max_speed;
    float max_acceleration;
    bool alive; // true = vivant, false = mort
} Fish;

typedef struct {
    Vec2 self_position;
    Vec2 self_speed_vector;

    Vec2 *neighbor_position;
    Vec2 *neighbor_speed_vector;
    int nb_neighbor; // nombre de voisins perçus

    bool shark_visible; // true si le requin est visible par le possion
    Vec2 shark_position;
    Vec2 shark_speed_vector;
} FishPerception;

#endif