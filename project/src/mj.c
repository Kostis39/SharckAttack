#include "mj.h"
#include "vector.h"
#include "world.h"

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

void UpdateWorld(World *world, World *tmp_world) {
    for (int i = 0; i < world->nb_fish; i++) {
        // Copie de l'état actuel
        tmp_world->fishes[i] = world->fishes[i];

        FishPerception perception =
            get_fish_perception(&world->fishes[i], world);

        tmp_world->fishes[i].velocity =
            fish_choose_action(&world->fishes[i], &perception);

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