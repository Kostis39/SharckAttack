#include "mj.h"
#include "vector.h"
#include "world.h"

void UpdateWorld(World *world, World *tmp_world) {
    for (int i = 0; i < world->nb_fish; i++) {
        // Copie de l'état actuel
        tmp_world->fishes[i] = world->fishes[i];

        FishPerception perception =
            Get_fish_perception(&world->fishes[i], world);

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