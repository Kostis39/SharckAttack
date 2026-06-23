#ifndef RULES_SET_H
#define RULES_SET_H
#include "config.h"
#include <stdlib.h>

typedef struct {
    float separation;
    float alignment;
    float cohesion;
    float shark_avoidance;
    float collider_avoidance;
} RulesSetFish;

typedef struct {
    float biais;
    float center;    // attraction vers le centre du banc
    float alignment; // alignement avec la direction moyenne du banc
    float pursuit;   // poursuite du poisson le plus proche
} SharkTheta;        // Anciennement RulesSetShark

RulesSetFish *RulesSetFish_init();
SharkTheta *SharkTheta_init();

void RulesSetFish_destroy(RulesSetFish *rules_set);
void SharkTheta_destroy(SharkTheta *rules_set);

#endif
