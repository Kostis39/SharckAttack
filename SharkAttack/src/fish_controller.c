#include "fish_controller.h"
#include "config.h"
#include "vector.h"
#include "world.h"
#include <math.h>

Vector Rules_separation(Fish *self, Vector separation) {
    (void)self; // self n'est pas utilisé dans cette règle, mais on le garde en
                // paramètre pour la cohérence avec les autres règles
    return separation;
}

Vector Rules_alignment(Fish *self, Vector avg_velocity) {
    return Vector_sub(avg_velocity, self->velocity);
}

Vector Rules_cohesion(Fish *self, Vector center_of_mass) {
    return Vector_sub(center_of_mass, self->position);
}

Vector Rules_border_repulsion(Fish *self, int width, int height) {
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
    } else if (self->position.x > width - REPULSION_ZONE) {
        distance_border = width - self->position.x;
        ratio = 1.0f - (distance_border / REPULSION_ZONE);
        repulsion.x -= REPULSION_FACTOR * ratio * ratio;
    }

    // Bords haut/bas
    if (self->position.y < REPULSION_ZONE) {
        distance_border = self->position.y;
        ratio = 1.0f - (distance_border / REPULSION_ZONE);
        repulsion.y += REPULSION_FACTOR * ratio * ratio;
    } else if (self->position.y > height - REPULSION_ZONE) {
        distance_border = height - self->position.y;
        ratio = 1.0f - (distance_border / REPULSION_ZONE);
        repulsion.y -= REPULSION_FACTOR * ratio * ratio;
    }

    return repulsion;
}

Vector Rules_avoid_shark(Fish *self, Vector dist_shark) {
    if (Vector_length(dist_shark) > self->radius_cohesion) {
        return Vector_init();
    }

    float dist = Vector_length(dist_shark);

    // Plus le requin est proche, plus la force est grande
    float intensity = 1.0f - dist / self->radius_cohesion;

    return Vector_scale(dist_shark, intensity);
}

Vector fish_choose_action(FishPerception *p, RulesSetFish *theta, int width,
                          int height) {
    /** @brief Choix de la direction du poisson comme une combinaison linéaire
     * de vecteurs
     * @param p un pointeur vers la perception du poisson
     * @param theta ensemble de paramètres quantifiant le comportement du
     * poisson
     * @return weighted_velocity vecteur vitesse du poisson comprenant son
     * comportement
     * */
    Vector separation = Rules_separation(&p->self, p->separation);
    Vector alignement = Rules_alignment(&p->self, p->avg_velocity);
    Vector cohesion = Rules_cohesion(&p->self, p->center_of_mass);

    Vector repulsion = Rules_border_repulsion(&p->self, width, height);

    Vector avoid_shark = Rules_avoid_shark(&p->self, p->dist_shark);

    Vector collider_avoidance =
        Rules_separation(&p->self, p->collider_separation);

    // Combinaison des forces
    Vector weighted_velocity = Vector_init();
    weighted_velocity = Vector_add(weighted_velocity,
                                   Vector_scale(separation, theta->separation));
    weighted_velocity = Vector_add(weighted_velocity,
                                   Vector_scale(alignement, theta->alignment));
    weighted_velocity =
        Vector_add(weighted_velocity, Vector_scale(cohesion, theta->cohesion));

    weighted_velocity = Vector_add(weighted_velocity,
                                   Vector_scale(repulsion, REPULSION_FACTOR));
    weighted_velocity = Vector_add(
        weighted_velocity, Vector_scale(avoid_shark, theta->shark_avoidance));
    weighted_velocity =
        Vector_add(weighted_velocity,
                   Vector_scale(collider_avoidance, theta->collider_avoidance));

    return weighted_velocity;
}
