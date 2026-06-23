#include "config.h"
#include "theta_set.h"

RulesSetFish *RulesSetFish_init() {
    RulesSetFish *theta = malloc(sizeof(RulesSetFish));
    theta->alignment = ALIGNMENT;
    theta->cohesion = COHESION;
    theta->separation = SEPARATION;
    theta->shark_avoidance = SHARK_AVOIDANCE;
    theta->collider_avoidance = COLLIDER_AVOIDANCE;
    return theta;
}

SharkTheta *SharkTheta_init() {
    SharkTheta *theta = malloc(sizeof(SharkTheta));

    theta->biais = BIAIS;
    theta->center = CENTER;
    theta->alignment = SHARK_ALIGNEMENT;
    theta->pursuit = PURSUIT;
    return theta;
}

void RulesSetFish_destroy(RulesSetFish *theta) { free(theta); }
void SharkTheta_destroy(SharkTheta *theta) {
    if (theta != NULL)
        free(theta);
}
