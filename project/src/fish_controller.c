#include "fish_controller.h"
#include "vector.h"

Vector Rules_separation(Fish *self, FishPerception *p) {
    Vector newVelocity = {0, 0};

    return newVelocity;
}
Vector Rules_alignment(Fish *self, FishPerception *p);
Vector Rules_cohesion(Fish *self, FishPerception *p);

Vector fish_choose_action(Fish *fish, FishPerception *perception);
