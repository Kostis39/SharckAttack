#ifndef FISH_H
#define FISH_H

/* #include "config.h" */
#include "vector.h"
#include <stdbool.h>
/* #include <stdlib.h> */

typedef struct {
    Vector position;
    Vector velocity;
    bool alive; // true = vivant, false = mort
} Fish;

Fish Fish_create_random_pos(int width, int height);
// Fish *Fish_create_null_array(int nb_fish, int width, int height);
Fish *Fish_create_random_array(int nb_fish, int width, int height);

Fish *Fish_copy_array(Fish *fish_array, int nb_fish);

void Fish_destroy_array(Fish *fish_array);

#endif
