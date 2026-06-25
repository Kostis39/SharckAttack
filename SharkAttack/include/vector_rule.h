#ifndef VECTOR_RULE_H
#define VECTOR_RULE_H

#include "config.h"
#include <stdio.h>

/**
 * @brief Structure représentant un vecteur de poids pour les règles de
 * comportement. La taille du tableau est définie par la valeur de l'énumération
 * `Rules_Lenght` dans `config.h`. Ce vecteur permet de stocker et de manipuler
 * de manière générique les composantes liées aux différentes règles.
 */
typedef struct {
    float
        vect[Rules_Lenght]; /**< Tableau contenant les poids de chaque règle. */
} VectorRule;

/**
 * @brief Initialise un VectorRule avec toutes ses composantes à 0.
 * @return Un VectorRule mis à zéro.
 */
VectorRule VectorRule_init();

/**
 * @brief Affiche les composantes du vecteur sur la console sous la forme (v1,
 * v2, ...).
 * @param v Le vecteur de règles à afficher.
 */
void VectorRule_print(VectorRule v);

/**
 * @brief écrit dans un fichier les composantes du vecteur sous la
 * forme (v1, v2, ...).
 * @param file fichier dans lequel écrire
 * @param v Le vecteur de règles à afficher.
 */
void VectorRule_fprint(FILE *file, VectorRule v);

/**
 * @brief Calcule le produit scalaire entre deux VectorRule.
 * @param a Le premier vecteur.
 * @param b Le second vecteur.
 * @return Le résultat du produit scalaire.
 */
float VectorRule_dot_product(VectorRule a, VectorRule b);

/**
 * @brief Multiplie toutes les composantes d'un VectorRule par un scalaire.
 * @param v Le vecteur à multiplier.
 * @param scalar Le facteur de multiplication.
 * @return Un nouveau VectorRule contenant le résultat
 */
VectorRule VectorRule_scaled(VectorRule v, float scalar);

/**
 * @brief Additionne deux VectorRule composante par composante.
 * @param a Le premier vecteur.
 * @param b Le second vecteur.
 * @return Un nouveau VectorRule contenant le résultat.
 */
VectorRule VectorRule_add(VectorRule a, VectorRule b);

/**
 * @brief soustrait deux VectorRule composante par composante.
 * @param a Le premier vecteur.
 * @param b Le second vecteur.
 * @return Un nouveau VectorRule contenant le résultat.
 */
VectorRule VectorRule_sub(VectorRule a, VectorRule b);

#endif