#include "fish_controller.h"
#include "config.h"
#include "vector.h"
#include <math.h>

Vector Rules_separation(Fish *self, Fish *near, int nb) {
    if (nb == 0)
        return Vector_init();

    Vector close = Vector_init();

    for (int i = 0; i < nb; ++i) {
        close = Vector_add(close, Vector_sub(self->position, near[i].position));
    }

    return close;
}

Vector Rules_alignment(Fish *self, Fish *med, int nb) {
    if (nb == 0)
        return Vector_init();

    Vector avg_vel = Vector_init();
    for (int i = 0; i < nb; ++i) {
        avg_vel = Vector_add(avg_vel, med[i].velocity);
    }
    avg_vel = Vector_scale(avg_vel, 1.0f / nb);

    return Vector_sub(avg_vel, self->velocity);
}

Vector Rules_cohesion(Fish *self, Fish *far, int nb) {
    if (nb == 0)
        return Vector_init();

    Vector center = Vector_init();
    for (int i = 0; i < nb; ++i) {
        center = Vector_add(center, far[i].position);
    }
    center = Vector_scale(center, 1.0f / nb);

    return Vector_sub(center, self->position);
}

Vector fish_choose_action(FishPerception *p) {
    Vector separation =
        Rules_separation(&p->self, p->neighbor_separation, p->nb_sep);
    Vector alignement =
        Rules_alignment(&p->self, p->neighbor_alignment, p->nb_align);
    Vector cohesion =
        Rules_cohesion(&p->self, p->neighbor_cohesion, p->nb_cohes);

    // Combinaison 3 trois forces
    Vector weighted_velocity = Vector_init();
    weighted_velocity =
        Vector_add(weighted_velocity, Vector_scale(separation, SEPARATION));
    weighted_velocity =
        Vector_add(weighted_velocity, Vector_scale(alignement, ALIGNMENT));
    weighted_velocity =
        Vector_add(weighted_velocity, Vector_scale(cohesion, COHESION));

    weighted_velocity = Vector_add(p->self.velocity,
                                   Vector_scale(weighted_velocity, TURN_SPEED));

    weighted_velocity = Vector_limit(weighted_velocity, FISH_SPEED_MAX);

    float speed = Vector_length(weighted_velocity);

    if (speed > FISH_SPEED_MAX) {
        weighted_velocity =
            Vector_scale(Vector_normalize(weighted_velocity), FISH_SPEED_MAX);
    } else if (speed < FISH_SPEED_MIN) {
        weighted_velocity =
            Vector_scale(Vector_normalize(weighted_velocity), FISH_SPEED_MIN);
    }

    return weighted_velocity;
}