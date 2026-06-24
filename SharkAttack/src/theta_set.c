#include "theta_set.h"
#include "config.h"
#include "input_output.h"

void SharkPhi_print(SharkPhi phi) {
    printf("Phi.x :");
    VectorRule_print(phi.x);
    printf(" Phi.y :");
    VectorRule_print(phi.x);
}

void SharkPhi_add_vector(SharkPhi *shark_phi, Vector vect,
                         enum Rules rule_to_apply) {
    shark_phi->x.vect[rule_to_apply] = vect.x;
    shark_phi->y.vect[rule_to_apply] = vect.y;
}

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
    VectorRule_print(*theta);
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
    for (int i = 0; i < Rules_Lenght; ++i) {
        result += a.vect[i] * b.vect[i];
    }

    return result;
}

VectorRule VectorRule_scaled(VectorRule v, float scalar) {
    for (int i = 0; i < Rules_Lenght; ++i) {
        v.vect[i] *= scalar;
    }

    return v;
}

VectorRule VectorRule_add(VectorRule a, VectorRule b) {
    VectorRule result;
    for (int i = 0; i < Rules_Lenght; ++i) {
        result.vect[i] = a.vect[i] + b.vect[i];
    }

    return result;
}