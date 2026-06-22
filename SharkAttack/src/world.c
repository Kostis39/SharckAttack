#include "world.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

World *World_init(int width, int height, int nb_fish, int nb_colliders) {
    srand(time(NULL));
    int seed = rand();
    World *new_world = calloc(1, sizeof(World));
    new_world->width = width;
    new_world->height = height;
    new_world->nb_fish = nb_fish;
    new_world->fishes = Fish_create_random_array(nb_fish, width, height);
    new_world->shark = Shark_createRandom(width, height);
    new_world->colliders =
        Colliders_random_array(seed, width, height, width / COLLIDER_RATIO,
                               height / COLLIDER_RATIO, nb_colliders);
    new_world->rules_set_fish = RulesSetFish_init();
    new_world->rules_set_shark = RulesSetShark_init();

    return new_world;
}

void World_destroy(World *world) {
    Fish_destroy_array(world->fishes);
    Shark_destroy(world->shark);
    Colliders_destroy_array(world->colliders);
    RulesSetFish_destroy(world->rules_set_fish);
    RulesSetShark_destroy(world->rules_set_shark);
    free(world);
    world = NULL;
}

void World_replace(World *world, World *world_tmp) {
    Fish_destroy_array(world->fishes);
    world->nb_fish = world_tmp->nb_fish;
    world->fishes = Fish_copy_array(world_tmp->fishes, world_tmp->nb_fish);
    world_tmp->colliders = world_tmp->colliders;
    Shark_copy(world->shark, world_tmp->shark);
}
