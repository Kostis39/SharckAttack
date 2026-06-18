#ifndef WORLD_H
#define WORLD_H

#include "fish.h"
#include "shark.h"

typedef struct {
    float width;
    float height;

    Fish *fishes;
    int nb_fish;

    Shark shark;
} World;

#endif