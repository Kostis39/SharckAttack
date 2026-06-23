#include "rules_set.h"
#include "config.h"

RulesSetFish *RulesSetFish_init() {
    RulesSetFish *rules_set = malloc(sizeof(RulesSetFish));
    rules_set->alignment = ALIGNMENT;
    rules_set->cohesion = COHESION;
    rules_set->separation = SEPARATION;
    rules_set->shark_avoidance = SHARK_AVOIDANCE;
    rules_set->collider_avoidance = COLLIDER_AVOIDANCE;
    return rules_set;
}

SharkTheta *SharkTheta_init() {
    SharkTheta *rules_set = malloc(sizeof(SharkTheta));

    rules_set->biais = BIAIS;
    rules_set->center = CENTER;
    rules_set->alignment = SHARK_ALIGNEMENT;
    rules_set->pursuit = PURSUIT;
    return rules_set;
}

void RulesSetFish_destroy(RulesSetFish *rules_set) { free(rules_set); }
void SharkTheta_destroy(SharkTheta *rules_set) {
    if (rules_set != NULL)
        free(rules_set);
}
