#include "mj.h"
#include "vector.h"
#include "world.h"
#include <math.h>

FishPerception get_fish_perception(Fish *fish, World *world) {
    FishPerception perception;
    perception.self = *fish;

    perception.neighbor_separation = calloc(FISH_NB, sizeof(Fish));
    perception.nb_sep = 0;
    perception.neighbor_alignment = calloc(FISH_NB, sizeof(Fish));
    perception.nb_align = 0;
    perception.neighbor_cohesion = calloc(FISH_NB, sizeof(Fish));
    perception.nb_cohes = 0;

    perception.shark_visible = false;
    perception.width = world->width;
    perception.height = world->height;

    if (world->shark != NULL) {
        float distance = Vector_distance(fish->position, world->shark->pos);
        if (distance <= PERCEPTION_RADIUS) {
            perception.shark_visible = true;
            perception.shark_position = world->shark->pos;
            perception.shark_velocity = world->shark->velocity;
        }
    }

    // Les trois zones de perception sont concentriques et s'enchaînent :
    // [0, separation_limit] -> séparation
    // ]separation_limit, alignment_limit] -> alignement
    // ]alignment_limit, cohesion_limit] -> cohésion
    float separation_limit = fish->radius_separation;
    float alignment_limit = separation_limit + fish->radius_alignement;
    float cohesion_limit = alignment_limit + fish->radius_cohesion;

    for (int i = 0; i < world->nb_fish; i++) {
        Fish *other = &world->fishes[i];

        if (other == fish || !other->is_alive) {
            continue;
        }

        Vector to_other = Vector_sub(other->position, fish->position);
        float distance = Vector_length(to_other);

        float angle = Vector_angle(fish->velocity, to_other);
        if (fabsf(angle) > fish->vision_angle / 2.0f) {
            continue;
        }

        if (distance <= separation_limit) {
            perception.neighbor_separation[perception.nb_sep] = *other;
            perception.nb_sep++;
        } else if (distance <= alignment_limit) {
            perception.neighbor_alignment[perception.nb_align] = *other;
            perception.nb_align++;
        } else if (distance <= cohesion_limit) {
            perception.neighbor_cohesion[perception.nb_cohes] = *other;
            perception.nb_cohes++;
        }
    }

    return perception;
}

void FishPerception_destroy(FishPerception *perception) {
    free(perception->neighbor_separation);
    free(perception->neighbor_alignment);
    free(perception->neighbor_cohesion);
}
void UpdateWorld(World *world, World *tmp_world) {
    for (int i = 0; i < world->nb_fish; i++) {
        // Copie de l'état actuel
        tmp_world->fishes[i] = world->fishes[i];

        FishPerception perception =
            get_fish_perception(&world->fishes[i], world);

        tmp_world->fishes[i].velocity = fish_choose_action(&perception);

        // Mise à jour de la position
        tmp_world->fishes[i].position = Vector_add(
            tmp_world->fishes[i].position, tmp_world->fishes[i].velocity);
    }

    // // Copie de l'état actuel
    // tmp_world->shark = world->shark;

    // SharkPerception shark_perception = Get_shark_perception(world);

    // tmp_world->shark.velocity =
    //     shark_choose_action(&world->shark, &shark_perception);

    // // Mise à jour de la position
    // tmp_world->shark.position =
    //     Vector_add(tmp_world->shark.position, tmp_world->shark.velocity);
}