#ifndef THETA_SET_H
#define THETA_SET_H
#include "config.h"
#include "vector.h"
#include "vector_rule.h"
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

void SharkPhi_print(SharkPhi shark_phi);

void SharkPhi_add_vector(SharkPhi *shark_phi, Vector vect,
                         enum Rules rule_to_apply);

RulesSetFish *RulesSetFish_init();
VectorRule *SharkTheta_init();

void RulesSetFish_destroy(RulesSetFish *theta);
void SharkTheta_destroy(VectorRule *theta);

#endif
