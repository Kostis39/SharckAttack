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

/**
 * @brief affiche les valeurs des règles phi du requin (composantes x et y)
 * @param phi structure SharkPhi
 */
void SharkPhi_print(SharkPhi shark_phi);

/**
 * @brief ajoute un vecteur à une règle phi du requin
 * @param shark_phi pointeur vers la structure SharkPhi à modifier
 * @param vect vecteur à ajouter
 * @param rule_to_apply règle cible concernée
 */
void SharkPhi_add_vector(SharkPhi *shark_phi, Vector vect,
                         enum Rules rule_to_apply);

/**
 * @brief initialise un ensemble de règles pour les poissons avec les valeurs
 * par défaut
 * @return pointeur vers la structure RulesSetFish allouée dynamiquement
 */
RulesSetFish *RulesSetFish_init();

/**
 * @brief initialise le vecteur theta du requin en chargeant les valeurs
 * depuis un fichier
 * @return pointeur vers le VectorRule alloué dynamiquement
 */
VectorRule *SharkTheta_init();

/**
 * @brief libère la mémoire d'un ensemble de règles pour les poissons
 * @param theta pointeur vers la structure RulesSetFish à libérer
 */
void RulesSetFish_destroy(RulesSetFish *theta);

/**
 * @brief libère la mémoire du vecteur theta du requin
 * @param theta pointeur vers le VectorRule à libérer
 */
void SharkTheta_destroy(VectorRule *theta);

#endif
