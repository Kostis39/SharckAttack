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

VectorRule VectorRule_init();

void VectorRule_print(VectorRule v);
void VectorRule_fprint(FILE *file, VectorRule v);

float VectorRule_dot_product(VectorRule a, VectorRule b);
VectorRule VectorRule_scaled(VectorRule v, float scalar);

VectorRule VectorRule_add(VectorRule a, VectorRule b);
VectorRule VectorRule_sub(VectorRule a, VectorRule b);

#endif