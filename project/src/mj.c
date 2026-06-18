#include "mj.h"

FishPerception get_fish_perception(Fish *fish, World *world) {
    FishPerception perception;
    perception.self = *fish;

    perception.neighbor_separation = calloc(FISH_NB, sizeof(Fish));
    perception.nb_neighbor_separation = 0;
    perception.neighbor_alignment = calloc(FISH_NB, sizeof(Fish));
    perception.nb_neighbor_alignment = 0;
    perception.neighbor_cohesion = calloc(FISH_NB, sizeof(Fish));
    perception.nb_neighbor_cohesion = 0;

    perception.shark_visible = false;
    perception.width = world->width;
    perception.height = world->height;
    /**
    for (int i = 0; i < world->nb_fish; i++) {
    }*/

    if (world->shark != NULL) {
        float distance = Vector_distance(fish->position, world->shark->pos);
        if (distance <= PERCEPTION_RADIUS) {
            perception.shark_visible = true;
            perception.shark_position = world->shark->pos;
            perception.shark_velocity = world->shark->velocity;
        }
    }

    return perception;
}

void FishPerception_destroy(FishPerception *perception) {
    free(perception->neighbor_separation);
    free(perception->neighbor_alignment);
    free(perception->neighbor_cohesion);
}