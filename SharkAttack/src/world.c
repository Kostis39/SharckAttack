#include "world.h"

World *World_init(int width, int height, int nb_fish, int nb_colliders,
                  VectorRule theta_shark, float sigma, int nb_occurrence,
                  bool is_player, bool learn, int nb_sharks) {
    World *new_world = calloc(1, sizeof(World));
    new_world->width = width;
    new_world->height = height;
    new_world->nb_fish = nb_fish;
    new_world->nb_colliders = nb_colliders;
    new_world->fishes = Fish_create_random_array(nb_fish, width, height);

    // Allocation du tableau de requins
    new_world->sharks = calloc(nb_sharks, sizeof(Shark *));
    new_world->trajectory = calloc(nb_sharks, sizeof(Trajectory *));

    new_world->sharks[0] = Shark_create(width, height, is_player);
    new_world->trajectory[0] = Trajectory_init();
    for (int i = 1; i < nb_sharks; i++) {
        new_world->sharks[i] = Shark_create(width, height, false);
        new_world->trajectory[i] = Trajectory_init();
    }
    new_world->nb_sharks = nb_sharks;

    new_world->colliders =
        Colliders_random_array(width, height, width / COLLIDER_RATIO,
                               height / COLLIDER_RATIO, nb_colliders, 1);
    new_world->fish_eaten = 0;
    new_world->theta_fish = RulesSetFish_init();
    new_world->theta_shark = theta_shark;
    new_world->sigma = sigma;
    new_world->nb_occurrence = nb_occurrence;
    new_world->learn = learn;

    return new_world;
}

void World_destroy(World *world) {
    Fish_destroy_array(world->fishes);
    for (int i = 0; i < world->nb_sharks; i++) {
        Shark_destroy(world->sharks[i]);
        Trajectory_destroy(world->trajectory[i]);
    }
    Colliders_destroy_array(world->colliders);
    RulesSetFish_destroy(world->theta_fish);
    free(world->sharks);
    free(world->trajectory);
    free(world);
    world = NULL;
}

bool World_create_tmp(const World *world, World *tmp_world) {
    if (!world || !tmp_world) {
        return false;
    }

    tmp_world->width = world->width;
    tmp_world->height = world->height;
    tmp_world->nb_fish = world->nb_fish;
    tmp_world->fish_eaten = world->fish_eaten;
    tmp_world->trajectory = world->trajectory;

    tmp_world->nb_sharks = world->nb_sharks;
    tmp_world->sharks = calloc(world->nb_sharks, sizeof(Shark *));
    if (!tmp_world->sharks) {
        fprintf(stderr, "World_create_temp: échec d'allocation des poissons\n");
        return false;
    }

    tmp_world->fishes = calloc(tmp_world->nb_fish, sizeof(Fish));
    if (!tmp_world->fishes) {
        fprintf(stderr, "World_create_temp: échec d'allocation des poissons\n");
        return false;
    }

    for (int i = 0; i < tmp_world->nb_sharks; i++) {
        tmp_world->sharks[i] = calloc(1, sizeof(Shark));
        if (!tmp_world->sharks[i]) {
            // Libérer les requins déjà alloués
            for (int j = 0; j < i; j++) {
                free(tmp_world->sharks[j]);
            }
            free(tmp_world->sharks);
            free(tmp_world->fishes);
            fprintf(stderr, "Erreur allocation requin %d\n", i);
            return false;
        }
    }

    return true;
}

void World_swap_data(World *world, World *tmp_world) {
    if (!world || !tmp_world) {
        fprintf(stderr, "World_swap_data: pointeur NULL\n");
        return;
    }

    // Poissons
    free(world->fishes);
    world->fishes = tmp_world->fishes;
    world->nb_fish = tmp_world->nb_fish;

    // Requins
    for (int i = 0; i < world->nb_sharks; i++) {
        free(world->sharks[i]);                  // Libère l'ancien requin
        world->sharks[i] = tmp_world->sharks[i]; // Prend le nouveau
    }
    free(tmp_world->sharks);

    world->fish_eaten = tmp_world->fish_eaten;
}
