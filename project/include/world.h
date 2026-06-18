#ifndef WORLD_H
#define WORLD_H

#include "vector.h"
#include "fish.h"
#include "shark.h"

typedef struct {
    int width;
    int height;

    Fish *fishes;
    int nb_fish;

    Shark *shark;
} World;

World *World_init(int width, int height, int nb_fish);
void World_destroy(World *world);

#endif
