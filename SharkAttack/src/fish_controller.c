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

Vector Rules_border_repulsion(Fish *self) {
    Vector repulsion = Vector_init();
    float ratio;
    float distance_border;

    // Bords gauche/droit
    if (self->position.x < REPULSION_ZONE) {
        distance_border = self->position.x;
        ratio = 1.0f - (distance_border / REPULSION_ZONE);
        repulsion.x += REPULSION_FACTOR * ratio *
                       ratio; // ratio au carré pour augmenter la répulsion
                              // rapidepment proche du bord
    } else if (self->position.x > WIDTH - REPULSION_ZONE) {
        distance_border = WIDTH - self->position.x;
        ratio = 1.0f - (distance_border / REPULSION_ZONE);
        repulsion.x -= REPULSION_FACTOR * ratio * ratio;
    }

    // Bords haut/bas
    if (self->position.y < REPULSION_ZONE) {
        distance_border = self->position.y;
        ratio = 1.0f - (distance_border / REPULSION_ZONE);
        repulsion.y += REPULSION_FACTOR * ratio * ratio;
    } else if (self->position.y > HEIGHT - REPULSION_ZONE) {
        distance_border = HEIGHT - self->position.y;
        ratio = 1.0f - (distance_border / REPULSION_ZONE);
        repulsion.y -= REPULSION_FACTOR * ratio * ratio;
    }

    return repulsion;
}

Vector Rules_avoid_shark(Fish *self, Vector shark_position,
                         bool shark_visible) {
    if (!shark_visible) {
        return Vector_init();
    }

    Vector away_from_shark = Vector_sub(self->position, shark_position);
    float dist = Vector_length(away_from_shark);

    // Plus le requin est proche, plus la force est grande
    float intensity = SHARK_AVOIDANCE * (1.0f - dist / RADIUS_SHARK_VISIBILITY);

    return Vector_scale(away_from_shark, intensity);
}

Vector fish_choose_action(FishPerception *p, RulesSetFish *rules_set) {
    Vector separation =
        Rules_separation(&p->self, p->neighbor_separation, p->nb_sep);
    Vector alignement =
        Rules_alignment(&p->self, p->neighbor_alignment, p->nb_align);
    Vector cohesion =
        Rules_cohesion(&p->self, p->neighbor_cohesion, p->nb_cohes);

    Vector repulsion = Rules_border_repulsion(&p->self);

    Vector avoid_shark =
        Rules_avoid_shark(&p->self, p->shark.pos, p->shark_visible);

    // Combinaison 3 trois forces
    Vector weighted_velocity = Vector_init();
    weighted_velocity = Vector_add(
        weighted_velocity, Vector_scale(separation, rules_set->separation));
    weighted_velocity = Vector_add(
        weighted_velocity, Vector_scale(alignement, rules_set->alignment));
    weighted_velocity = Vector_add(weighted_velocity,
                                   Vector_scale(cohesion, rules_set->cohesion));

    weighted_velocity = Vector_add(weighted_velocity, repulsion);
    weighted_velocity = Vector_add(weighted_velocity, avoid_shark);

    return weighted_velocity;
}