#ifndef RULES_SET_H
#define RULES_SET_H
#include "config.h"
#include <stdlib.h>

typedef struct {
    float separation;
    float alignment;
    float cohesion;
    float shark_avoidance;
} RulesSetFish;

typedef struct {
    float biais;
    float center;    // attraction vers le centre du banc
    float alignment; // alignement avec la direction moyenne du banc
    float pursuit;   // poursuite du poisson le plus proche
} SharkTeta;         // Anciennement RulesSetShark

RulesSetFish *RulesSetFish_init();
SharkTeta *SharkTeta_init();

void RulesSetFish_destroy(RulesSetFish *rules_set);
void SharkTeta_destroy(SharkTeta *rules_set);

#endif