#ifndef COLLIDER_H
#define COLLIDER_H

#include "vector.h"
#include "world.h"
#include <stdbool.h>

typedef struct {
    Vector bounding_box[2]; /**< Bounding box de l'objet de collision, coin
                               supérieur gauche et coin inférieur droit*/
    /* bool is_enabled; */
} Collider;

#endif

float random_float(float min, float max);
Collider Instantiate_collider(Vector pos, float w, float h);
Collider *Colliders_random_array(int seed, int x_max, int y_max, int w_max,
                                 int h_max, int count);
Collider *Colliders_copy_array(Collider *collider, int count);

int Colliders_destroy_array(Collider *array);

void Shark_collision(World *world);
