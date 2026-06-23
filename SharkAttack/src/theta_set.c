#include "theta_set.h"
#include "config.h"

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

    theta->biais.x = BIAIS;
    theta->biais.y = BIAIS;
    theta->center.x = CENTER;
    theta->center.y = CENTER;
    theta->alignment.x = SHARK_ALIGNEMENT;
    theta->alignment.y = SHARK_ALIGNEMENT;
    theta->pursuit.x = PURSUIT;
    theta->pursuit.y = PURSUIT;
    return theta;
}

void RulesSetFish_destroy(RulesSetFish *theta) { free(theta); }
void SharkTheta_destroy(SharkTheta *theta) {
    if (theta != NULL)
        free(theta);
}
