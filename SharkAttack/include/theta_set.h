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
    VectorRule x;
    VectorRule y;
} SharkPhi;

RulesSetFish *RulesSetFish_init();
VectorRule *SharkTheta_init();

void RulesSetFish_destroy(RulesSetFish *theta);
void SharkTheta_destroy(VectorRule *theta);

float Dot_product(VectorRule a, VectorRule b);
VectorRule Vector_rule_scaled(VectorRule v, float scalar);
VectorRule Vector_rule_add(VectorRule a, VectorRule b);

#endif
