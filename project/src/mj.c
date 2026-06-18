#include "mj.h"

FishPerception get_fish_perception(Fish *fish, World *world) {
    FishPerception perception;
    perception.self = *fish;
    perception.nb_neighbor = 0;
    perception.neighbor_position = malloc(sizeof(Vector) * MAX_VOISINS);
    perception.neighbor_speed_vector = malloc(sizeof(Vector) * MAX_VOISINS);
    perception.shark_visible = false;
    perception.width = world->width;
    perception.height = world->height;

    for (int i = 0; i < world->nb_fish; i++) {
        Fish *other_fish = &world->fishes[i];
        if (other_fish != fish && other_fish->is_alive) {
            float distance =
                Vector_distance(fish->position, other_fish->position);
            if (distance <= PERCEPTION_RADIUS &&
                perception.nb_neighbor < MAX_VOISINS) {
                perception.neighbor_position[perception.nb_neighbor] =
                    other_fish->position;
                perception.neighbor_speed_vector[perception.nb_neighbor] =
                    other_fish->velocity;
                perception.nb_neighbor++;
            }
        }
    }

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