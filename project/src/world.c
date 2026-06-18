#include "world.h"

World *World_init(int width, int height, int nb_fish) {
    World *new_world = calloc(1, sizeof(World));
    new_world->width = width;
    new_world->height = height;
    new_world->nb_fish = nb_fish;
    new_world->fishes = Fish_create_random_array(nb_fish, width, height);
    new_world->shark = SharkCreateRandom(width, height);
    return new_world;
}

void World_destroy(World *world) {
    Fish_destroy(world->fishes, world->nb_fish);
    SharkDestroy(world->shark);
    free(world);
}
