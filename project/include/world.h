#ifndef WORLD_H
#define WORLD_H

#include "fish.h"
#include "shark.h"
#include "vector.h"
#include <stdlib.h>

typedef struct {
    int width;
    int height;

    Fish *fishes;
    int nb_fish;

    Shark *shark;
} World;

World *World_init(int width, int height, int nb_fish);
void World_destroy(World *world);
void World_update(World *world, World *world_tmp);

#endif
