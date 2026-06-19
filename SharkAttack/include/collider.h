#ifndef COLLIDER_H
#define COLLIDER_H

#include "vector.h"
#include <stdbool.h>

typedef struct {
    Vector bounding_box[2]; /**< Bounding box de l'objet de collision, coin
                               supérieur gauche et coin inférieur droit*/
    /* bool is_enabled; */
} Collider;

#endif

Collider Instantiate_collider(Vector pos, int w, int h);
Collider *Colliders_random_array(int x_min, int y_min, int x_max, int y_max,
                                 int count);
Collider *Colliders_copy_array(int x_min, int y_min, int x_max, int y_max,
                               int count);

void Colliders_destroy_array(Collider *array, int count);
