#include "fish.h"
#include "config.h"
#include "utils.h"

Fish Fish_create_random_pos(int width, int height) {
    Fish fish;
    fish.position.x = random_float(0, width);
    fish.position.y = random_float(0, height);
    fish.velocity.x = random_float(-FISH_SPEED_MAX, FISH_SPEED_MAX);
    fish.velocity.y = random_float(-FISH_SPEED_MAX, FISH_SPEED_MAX);
    fish.is_alive = true;
    fish.radius_separation = RADIUS_SEPARATION;
    fish.radius_alignement = RADIUS_ALIGNEMENT;
    fish.radius_cohesion = RADIUS_COHESION;
    fish.vision_angle = VISION_ANGLE;

    return fish;
}

Fish *Fish_create_random_array(int nb_fish, int width, int height) {
    Fish *fish_array = calloc(nb_fish, sizeof(Fish));
    for (int i = 0; i < nb_fish; i++) {
        fish_array[i] = Fish_create_random_pos(width, height);
    }
    return fish_array;
}

/**
 * @brief crée un tableau de poissons sans les initialiser
 * @param nb_fish nombre de poissons
 * @return pointeur vers le tableau alloué dynamiquement
 */
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

void fish_apply_action(Fish *fish, Vector weighted_velocity) {
    Vector new_velocity =
        Vector_add(fish->velocity, Vector_scale(weighted_velocity, TURN_SPEED));

    float speed = Vector_length(new_velocity);

    if (speed > FISH_SPEED_MAX) {
        new_velocity =
            Vector_scale(Vector_normalize(new_velocity), FISH_SPEED_MAX);
    } else if (speed < FISH_SPEED_MIN) {
        new_velocity =
            Vector_scale(Vector_normalize(new_velocity), FISH_SPEED_MIN);
    }

    // Mise à jour
    fish->velocity = new_velocity;
    fish->position = Vector_add(fish->position, fish->velocity);
}
