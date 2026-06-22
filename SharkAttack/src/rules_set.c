#include "rules_set.h"

RulesSetFish *RulesSetFish_init() {
    RulesSetFish *rules_set = malloc(sizeof(RulesSetFish));
    rules_set->alignment = ALIGNMENT;
    rules_set->cohesion = COHESION;
    rules_set->separation = SEPARATION;
    rules_set->shark_avoidance = SHARK_AVOIDANCE;
    return rules_set;
}

RulesSetShark *RulesSetShark_init() {
    RulesSetShark *rules_set = malloc(sizeof(RulesSetShark));

    rules_set->center = CENTER;
    rules_set->alignment = ALIGNEMENT;
    rules_set->pursuit = PURSUIT;
    return rules_set;
}

void RulesSetFish_destroy(RulesSetFish *rules_set) { free(rules_set); }
void RulesSetShark_destroy(RulesSetShark *rules_set) {
    if (rules_set != NULL)
        free(rules_set);
}