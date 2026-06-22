#include "rules_set.h"

RulesSetFish *RulesSetFish_init() {
    RulesSetFish *rules_set = malloc(sizeof(RulesSetFish));
    rules_set->alignment = ALIGNMENT;
    rules_set->cohesion = COHESION;
    rules_set->separation = SEPARATION;
    rules_set->shark_avoidance_factor = SHARK_AVOIDANCE_FACTOR;
    return rules_set;
}

RulesSetShark *RulesSetShark_init() { return NULL; }

void RulesSetFish_destroy(RulesSetFish *rules_set) { free(rules_set); }
void RulesSetShark_destroy(RulesSetShark *rules_set) {
    if (rules_set != NULL)
        free(rules_set);
}