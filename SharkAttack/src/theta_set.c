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

// SharkTheta *SharkTheta_init() {
//     SharkTheta *theta = malloc(sizeof(SharkTheta));

//     theta->x.center = CENTER;
//     theta->y.center = CENTER;
//     theta->x.alignment = SHARK_ALIGNEMENT;
//     theta->y.alignment = SHARK_ALIGNEMENT;
//     theta->x.pursuit = PURSUIT;
//     theta->y.pursuit = PURSUIT;
//     return theta;
// }

SharkTheta *SharkTheta_init() {
    SharkTheta *theta = malloc(sizeof(SharkTheta));

    if (!load_theta(theta, THETA_FILE)) {
        fprintf(stderr, "Erreur: échec du chargement des paramètres\n");
        return NULL;
    }

    printf("\n=== Theta ===\n");
    printf("theta_x = (%f, %f, %f)\n", theta->x.center, theta->x.alignment,
           theta->x.pursuit);
    printf("theta_y = (%f, %f, %f)\n", theta->y.center, theta->y.alignment,
           theta->y.pursuit);
    printf("\n");

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