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

    theta->theta_x.center = CENTER;
    theta->theta_y.center = CENTER;
    theta->theta_x.alignment = SHARK_ALIGNEMENT;
    theta->theta_y.alignment = SHARK_ALIGNEMENT;
    theta->theta_x.pursuit = PURSUIT;
    theta->theta_y.pursuit = PURSUIT;
    return theta;
}

void RulesSetFish_destroy(RulesSetFish *theta) { free(theta); }
void SharkTheta_destroy(SharkTheta *theta) {
    if (theta != NULL)
        free(theta);
}

float Dot_product(VectorRule a, VectorRule b) {
    float result = 0.0f;

    result += a.center * b.center;
    result += a.alignment * b.alignment;
    result += a.pursuit * b.pursuit;

    return result;
}

VectorRule Vector_rule_scaled(VectorRule v, float scalar) {
    v.center = v.center * scalar;
    v.alignment = v.alignment * scalar;
    v.pursuit = v.pursuit * scalar;

    return v;
}

VectorRule Vector_rule_add(VectorRule a, VectorRule b) {
    VectorRule result;

    result.center = a.center + b.center;
    result.alignment = a.alignment + b.alignment;
    result.pursuit = a.pursuit + b.pursuit;

    return result;
}