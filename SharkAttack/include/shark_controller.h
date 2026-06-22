#ifndef AGENT_SHARK_H
#define AGENT_SHARK_H

#include "fish.h"
#include "shark.h"
#include "vector.h"

typedef struct {
    Shark self;

    Fish closest_fish;
    bool has_prey;

    Vector center_of_mass;
    Vector avg_velocity;

    int width;
    int height;
} SharkPerception;

Vector shark_choose_action(SharkPerception *perception);

#endif