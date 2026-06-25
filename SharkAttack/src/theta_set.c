#include "theta_set.h"
#include "config.h"
#include "input_output.h"

/**
 * @brief affiche les valeurs des règles phi du requin (composantes x et y)
 * @param phi structure SharkPhi
 */
void SharkPhi_print(SharkPhi phi) {
    printf("Phi.x :");
    VectorRule_print(phi.x);
    printf(" Phi.y :");
    VectorRule_print(phi.x);
}

/**
 * @brief ajoute un vecteur à une règle phi du requin
 * @param shark_phi pointeur vers la structure SharkPhi à modifier
 * @param vect vecteur à ajouter
 * @param rule_to_apply règle cible concernée
 */
void SharkPhi_add_vector(SharkPhi *shark_phi, Vector vect,
                         enum Rules rule_to_apply) {
    shark_phi->x.vect[rule_to_apply] = vect.x;
    shark_phi->y.vect[rule_to_apply] = vect.y;
}

/**
 * @brief initialise un ensemble de règles pour les poissons avec les valeurs
 * par défaut
 * @return pointeur vers la structure RulesSetFish allouée dynamiquement
 */
RulesSetFish *RulesSetFish_init() {
    RulesSetFish *theta = malloc(sizeof(RulesSetFish));
    theta->alignment = ALIGNMENT;
    theta->cohesion = COHESION;
    theta->separation = SEPARATION;
    theta->shark_avoidance = SHARK_AVOIDANCE;
    theta->collider_avoidance = COLLIDER_AVOIDANCE;
    return theta;
}

/**
 * @brief initialise le vecteur theta du requin en chargeant les valeurs
 * depuis un fichier
 * @return pointeur vers le VectorRule alloué dynamiquement
 */
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

/**
 * @brief libère la mémoire d'un ensemble de règles pour les poissons
 * @param theta pointeur vers la structure RulesSetFish à libérer
 */
void RulesSetFish_destroy(RulesSetFish *theta) { free(theta); }

/**
 * @brief libère la mémoire du vecteur theta du requin
 * @param theta pointeur vers le VectorRule à libérer
 */
void SharkTheta_destroy(VectorRule *theta) {
    if (theta != NULL)
        free(theta);
}