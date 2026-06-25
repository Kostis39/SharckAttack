#include "vector_rule.h"

/**
 * @brief Initialise un VectorRule avec toutes ses composantes à 0.
 * @return Un VectorRule mis à zéro.
 */
VectorRule VectorRule_init() {
    VectorRule rules;
    for (int i = 0; i < Rules_Lenght; ++i) {
        rules.vect[i] = 0.0f;
    }
    return rules;
}

/**
 * @brief Affiche les composantes du vecteur sur la console sous la forme (v1,
 * v2, ...).
 * @param v Le vecteur de règles à afficher.
 */
void VectorRule_print(VectorRule v) {
    printf("(");
    for (int i = 0; i < Rules_Lenght; ++i) {
        printf("%f", v.vect[i]);

        if (i < Rules_Lenght - 1) {
            printf(", ");
        }
    }
    printf(")");
}

/**
 * @brief écrit dans un fichier les composantes du vecteur sous la
 * forme (v1, v2, ...).
 * @param file fichier dans lequel écrire
 * @param v Le vecteur de règles à afficher.
 */
void VectorRule_fprint(FILE *file, VectorRule v) {
    fprintf(file, "(");
    for (int i = 0; i < Rules_Lenght; ++i) {
        fprintf(file, "%f", v.vect[i]);

        if (i < Rules_Lenght - 1) {
            fprintf(file, ", ");
        }
    }
    fprintf(file, ")");
}

/**
 * @brief Calcule le produit scalaire entre deux VectorRule.
 * @param a Le premier vecteur.
 * @param b Le second vecteur.
 * @return Le résultat du produit scalaire.
 */
float VectorRule_dot_product(VectorRule a, VectorRule b) {
    float result = 0.0f;
    for (int i = 0; i < Rules_Lenght; ++i) {
        result += a.vect[i] * b.vect[i];
    }

    return result;
}

/**
 * @brief Multiplie toutes les composantes d'un VectorRule par un scalaire.
 * @param v Le vecteur à multiplier.
 * @param scalar Le facteur de multiplication.
 * @return Un nouveau VectorRule contenant le résultat
 */
VectorRule VectorRule_scaled(VectorRule v, float scalar) {
    for (int i = 0; i < Rules_Lenght; ++i) {
        v.vect[i] *= scalar;
    }

    return v;
}

/**
 * @brief Additionne deux VectorRule composante par composante.
 * @param a Le premier vecteur.
 * @param b Le second vecteur.
 * @return Un nouveau VectorRule contenant le résultat.
 */
VectorRule VectorRule_add(VectorRule a, VectorRule b) {
    VectorRule result;
    for (int i = 0; i < Rules_Lenght; ++i) {
        result.vect[i] = a.vect[i] + b.vect[i];
    }
    return result;
}

/**
 * @brief soustrait deux VectorRule composante par composante.
 * @param a Le premier vecteur.
 * @param b Le second vecteur.
 * @return Un nouveau VectorRule contenant le résultat.
 */
VectorRule VectorRule_sub(VectorRule a, VectorRule b) {
    VectorRule result;
    for (int i = 0; i < Rules_Lenght; ++i) {
        result.vect[i] = a.vect[i] - b.vect[i];
    }
    return result;
}