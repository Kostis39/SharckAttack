#ifndef AGENT_SHARK_H
#define AGENT_SHARK_H

#include "fish.h"
#include "shark.h"
#include "vector.h"

typedef struct {
    Shark self;

    Fish *visible_fish;
    int nb_visible_fish;

    Fish closest_fish;
    float closest_prey_dist;

    Vector center_of_mass;
    Vector avg_velocity;

    int width;
    int height;
} SharkPerception;

Vector shark_choose_action(SharkPerception *perception);

#endif