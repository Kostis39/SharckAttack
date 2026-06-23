#ifndef THETA_SET_H
#define THETA_SET_H
#include "config.h"
#include "vector.h"
#include <stdlib.h>

typedef struct {
    float separation;
    float alignment;
    float cohesion;
    float shark_avoidance;
    float collider_avoidance;
} RulesSetFish;

typedef struct {
    Vector biais;
    Vector center;    // attraction vers le centre du banc
    Vector alignment; // alignement avec la direction moyenne du banc
    Vector pursuit;   // poursuite du poisson le plus proche
} SharkTheta;         // Anciennement RulesSetShark

RulesSetFish *RulesSetFish_init();
SharkTheta *SharkTheta_init();

void RulesSetFish_destroy(RulesSetFish *theta);
void SharkTheta_destroy(SharkTheta *theta);

#endif
