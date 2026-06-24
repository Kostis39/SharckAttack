#include "world.h"

World *World_init(int width, int height, int nb_fish, int nb_colliders,
                  VectorRule theta_shark, float sigma, bool is_player) {
    World *new_world = calloc(1, sizeof(World));
    new_world->width = width;
    new_world->height = height;
    new_world->nb_fish = nb_fish;
    new_world->nb_colliders = nb_colliders;
    new_world->fishes = Fish_create_random_array(nb_fish, width, height);
    new_world->shark = Shark_create(width, height, is_player);
    new_world->colliders =
        Colliders_random_array(width, height, width / COLLIDER_RATIO,
                               height / COLLIDER_RATIO, nb_colliders, 1);
    new_world->fish_eaten = 0;
    new_world->theta_fish = RulesSetFish_init();
    new_world->theta_shark = theta_shark;
    new_world->trajectory = Trajectory_init();
    new_world->sigma = sigma;

    return new_world;
}

void World_destroy(World *world) {
    Fish_destroy_array(world->fishes);
    Shark_destroy(world->shark);
    Colliders_destroy_array(world->colliders);
    RulesSetFish_destroy(world->theta_fish);
    Trajectory_destroy(world->trajectory);
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
