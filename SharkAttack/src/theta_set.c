#include "theta_set.h"
#include "config.h"
#include "input_output.h"

RulesSetFish *RulesSetFish_init() {
    RulesSetFish *theta = malloc(sizeof(RulesSetFish));
    theta->alignment = ALIGNMENT;
    theta->cohesion = COHESION;
    theta->separation = SEPARATION;
    theta->shark_avoidance = SHARK_AVOIDANCE;
    theta->collider_avoidance = COLLIDER_AVOIDANCE;
    return theta;
}

VectorRule *SharkTheta_init() {
    VectorRule *theta = calloc(1, sizeof(VectorRule));

    if (!load_theta(theta, THETA_FILE)) {
        fprintf(stderr, "Erreur: échec du chargement des paramètres\n");
        return NULL;
    }

    printf("\n=== Theta ===\n");
    printf("theta = (%f, %f, %f)\n", theta->center, theta->alignment,
           theta->pursuit);
    printf("\n");

    return theta;
}

void RulesSetFish_destroy(RulesSetFish *theta) { free(theta); }
void SharkTheta_destroy(VectorRule *theta) {
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