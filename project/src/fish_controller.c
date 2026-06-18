#include "fish_controller.h"
#include "vector.h"

/* float getDistance(Fish *f1, Fish *f2) { */

/* } */
Vector Rules_separation(Fish *near, int nb) {
    Vector centre = Vector_init();
    Vector escape = Vector_init();
    for (int i = 0; i < nb; ++i) {
        centre.x += near->position.x;
        centre.y += near->position.y;
    }
    centre.x /= nb;
    centre.y /= nb;
    float norm = Vector_norm(centre);
    escape.x = -centre.x / norm;
    escape.y = -centre.y / norm;
    return escape;
}
Vector Rules_alignment(Fish *med, int nb) {
    Vector avg = {0, 0};
    for (int i = 0; nb > i; ++i) {
        avg.x += med->velocity.x;
        avg.y += med->velocity.y;
    }
    avg.x /= nb;
    avg.y /= nb;
    return avg;
}
Vector Rules_cohesion(Fish *far, int nb) {
    Vector centre = Vector_init();
    Vector escape = Vector_init();
    for (int i = 0; i < nb; ++i) {
        centre.x += near->position.x;
        centre.y += near->position.y;
    }
    centre.x /= nb;
    centre.y /= nb;
    float norm = Vector_norm(centre);
    escape.x = -centre.x / norm;
    escape.y = -centre.y / norm;
    return escape;
}
Vector fish_choose_action(FishPerception *perception) {
    Vector weighted_velocity;
    return weighted_velocity;
}
