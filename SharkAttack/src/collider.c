#include "collider.h"

Collider Instantiate_collider(Vector pos, float w, float h) {
    Collider new_collider;
    new_collider.bounding_box[0].x = pos.x - w / 2;
    new_collider.bounding_box[0].y = pos.y - h / 2;

    new_collider.bounding_box[1].x = pos.x + w / 2;
    new_collider.bounding_box[1].y = pos.y + h / 2;
    return new_collider;
}

Collider *Colliders_random_array(int x_max, int y_max, int w_max, int h_max,
                                 int count, int gen_all_square) {
    Collider *array = calloc(count, sizeof(*array));
    if (!array) {
        return NULL;
    }
    Vector current_pos = Vector_init();
    float cur_w, cur_h;
    for (int i = 0; i < count; ++i) {
        cur_w = random_float(0, w_max);
        cur_h = random_float(0, h_max);

        // generer le centre
        current_pos.x = random_float(0, x_max);
        current_pos.y = random_float(0, y_max);
        if (!gen_all_square) {
            array[i] = Instantiate_collider(current_pos, cur_w,
                                            cur_h); // collider d'indice i
        } else {
            array[i] = Instantiate_collider(current_pos, cur_w,
                                            cur_w); // collider d'indice i
        }
    }
    return array;
}

int Colliders_destroy_array(Collider *array) {
    if (array) {
        free(array);
        return 0;
    }
    return -1;
}

Vector Collider_closest_point(Collider *c, Vector p) {
    Vector closest;
    closest.x = fmaxf(c->bounding_box[0].x, fminf(p.x, c->bounding_box[1].x));
    closest.y = fmaxf(c->bounding_box[0].y, fminf(p.y, c->bounding_box[1].y));
    return closest;
}

float Collider_distance_to_point(Collider *c, Vector p) {
    Vector closest = Collider_closest_point(c, p);
    return Vector_distance(p, closest);
}

Collider *Colliders_copy_array(Collider *collider, int count) {
    Collider *copy = malloc(count * sizeof(*collider));
    if (!(collider && copy)) {
        return NULL;
    }
    memcpy(copy, collider, count * sizeof(*collider));
    return copy;
}
