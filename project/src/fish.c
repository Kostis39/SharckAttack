#include "fish.h"

Fish Fish_create_random_pos(int width, int height) {
    Fish fish;
    fish.position.x = rand() % width;
    fish.position.y = rand() % height;
    fish.velocity.x = (rand() % FISH_SPEED_MAX) - (FISH_SPEED_MAX / 2);
    fish.velocity.y = (rand() % FISH_SPEED_MAX) - (FISH_SPEED_MAX / 2);
    fish.alive = true;
    return fish;
}

Fish *Fish_create_random_array(int nb_fish, int width, int height) {
    Fish *fish_array = calloc(nb_fish, sizeof(Fish));
    for (int i = 0; i < nb_fish; i++) {
        fish_array[i] = Fish_create_random_pos(width, height);
    }
    return fish_array;
}

Fish *Fish_create_null_array(int nb_fish) {
    Fish *fish_array = calloc(nb_fish, sizeof(Fish));
    return fish_array;
}

Fish *Fish_copy_array(Fish *fish_array, int nb_fish) {
    Fish *new_array = calloc(nb_fish, sizeof(Fish));
    for (int i = 0; i < nb_fish; i++) {
        new_array[i] = fish_array[i];
    }
    return new_array;
}

void Fish_destroy_array(Fish *fish_array) {
    free(fish_array);
    fish_array = NULL;
}