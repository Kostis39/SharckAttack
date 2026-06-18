#ifndef SHARK_H
#define SHARK_H

#include "fish.h"
#include "vector.h"

typedef struct {
    Vec2 position;
    Vec2 speed_vector;
    float max_speed;
    float max_acceleration;
} Shark;

#endif