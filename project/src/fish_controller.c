#include "fish_controller.h"
#include "config.h"
#include "vector.h"

/* float getDistance(Fish *f1, Fish *f2) { */

/* } */
Vector Rules_separation(Fish *near, int nb) {
    if (nb == 0)
        return Vector_init();

    Vector centre = Vector_init();
    Vector escape = Vector_init();
    for (int i = 0; i < nb; ++i) {
        centre.x += near[i].position.x;
        centre.y += near[i].position.y;
    }
    centre.x /= nb;
    centre.y /= nb;
    float norm = Vector_length(centre);
    escape.x = -centre.x / norm;
    escape.y = -centre.y / norm;
    return escape;
}
Vector Rules_alignment(Fish *med, int nb) {
    if (nb == 0)
        return Vector_init();

    Vector avg = Vector_init();
    for (int i = 0; nb > i; ++i) {
        avg.x += med[i].velocity.x;
        avg.y += med[i].velocity.y;
    }
    avg.x /= nb;
    avg.y /= nb;
    return avg;
}
Vector Rules_cohesion(Fish *far, int nb) {
    if (nb == 0)
        return Vector_init();

    Vector centre = Vector_init();
    Vector group = Vector_init();
    for (int i = 0; i < nb; ++i) {
        centre.x += far[i].position.x;
        centre.y += far[i].position.y;
    }
    centre.x /= nb;
    centre.y /= nb;
    float norm = Vector_length(centre);
    group.x = centre.x / norm;
    group.y = centre.y / norm;
    return group;
}
Vector fish_choose_action(FishPerception *p) {
    Vector separation = Rules_separation(p->neighbor_separation, p->nb_sep);
    Vector alignement = Rules_alignment(p->neighbor_alignment, p->nb_align);
    Vector cohesion = Rules_cohesion(p->neighbor_cohesion, p->nb_cohes);
    Vector weighted_velocity =
        Vector_add(Vector_scale(separation, SEPARATION),
                   Vector_add(Vector_scale(alignement, ALIGNMENT),
                              Vector_scale(cohesion, COHESION)));
    return weighted_velocity;
}
