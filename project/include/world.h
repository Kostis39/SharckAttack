#ifndef WORLD_H
#define WORLD_H

#include "fish.h"
#include "shark.h"

typedef struct {
    int width;
    int height;

    Fish *fishes;
    int nb_fish;

    Shark shark;
} World;

void WorldInit(World *world, int width, int height, int nb_fish);
void World_Destroy(World *world);

#endif