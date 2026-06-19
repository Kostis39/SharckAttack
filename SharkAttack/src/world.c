#include "world.h"
#include <stdlib.h>
#include <time.h>

World *World_init(int width, int height, int nb_fish) {
    srand(time(NULL));
    int seed = rand();
    World *new_world = calloc(1, sizeof(World));
    new_world->width = width;
    new_world->height = height;
    new_world->nb_fish = nb_fish;
    new_world->fishes = Fish_create_random_array(nb_fish, width, height);
    new_world->shark = Shark_createRandom(width, height);
    new_world->colliders = Colliders_random_array(
        int seed, int x_max, int y_max, int w_max, int h_max, int count);
    return new_world;
}

void World_destroy(World *world) {
    Fish_destroy_array(world->fishes);
    Shark_destroy(world->shark);
    free(world);
    world = NULL;
}

void World_replace(World *world, World *world_tmp) {
    Fish_destroy_array(world->fishes);
    world->nb_fish = world_tmp->nb_fish;
    world->fishes = Fish_copy_array(world_tmp->fishes, world_tmp->nb_fish);
    Shark_copy(world->shark, world_tmp->shark);
}
