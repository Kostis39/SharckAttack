#ifndef VECTOR_H
#define VECTOR_H

#include "config.h"
#include <math.h>
#include <stdio.h>

/**
 * @struct Vector
 * @brief Structure représentant un vecteur 2D.
 */
typedef struct {
    float x; /**< Composante x du vecteur */
    float y; /**< Composante y du vecteur */
} Vector;

void Vector_print(Vector v);

/**
 * @brief Initialise les composantes d'un vecteur à 0.
 *
 * @return Vector avec les composantes à 0.
 */
Vector Vector_init();

Vector Vector_add(Vector a, Vector b);
Vector Vector_sub(Vector a, Vector b);
Vector Vector_scale(Vector v, float k);

/**
 * @brief calcule le produit scalaire de deux vecteurs
 * @param a premier vecteur
 * @param b second vecteur
 * @return produit scalaire de a et b
 */
float Vector_dot(Vector a, Vector b);

/**
 * @brief calcule la norme d'un vecteur
 * @param v vecteur
 * @return norme du vecteur
 */
float Vector_length(Vector v);

/**
 * @brief Renvoie la norme du vecteur passé en paramètre au carrée
 *
 * @param v On calcule la norme de ce vecteur.
 * @return float Renvoie la norme au carré.
 */
float Vector_length2(Vector v);

/**
 * @brief Calcul la distance entre 2 vecteurs.
 */
float Vector_distance(Vector a, Vector b);

/**
 * @brief Normalise un vecteur (le rend unitaire / de norme 1)
 * @param v Vecteur à normaliser
 * @return Vecteur unitaire
 */
Vector Vector_normalize(Vector v);

/**
 * @brief Normalise un vecteur avec une valeur par défaut
 * @param v Vecteur à normaliser
 * @return Vecteur unitaire, ou (1, 0) si la norme est petite
 */
Vector Local_normalize(Vector v);

/**
 * @brief Fonction qui limite la longueur d'un vecteur à une valeur maximale
 * donnée. Si la longueur du vecteur dépasse cette valeur, le vecteur est
 * normalisé et mis à l'échelle pour correspondre à la longueur maximale. Si
 * la longueur du vecteur est inférieure ou égale à la longueur maximale, le
 * vecteur est retourné inchangé.
 *
 * @param v Le vecteur à limiter
 * @param max_length La longueur maximale autorisée pour le vecteur
 * @return Vector Le vecteur limité à la longueur maximale spécifiée
 */
Vector Vector_limit(Vector v, float max_length);

float Vector_angle(Vector a, Vector b);

/**
 * @brief Calcule l'angle entre deux vecteurs avec une approximation
 * d'Arctangente
 * @note Moins précis, plus rapide, à utiliser sur les poissons par
 * exemple
 * @param a premier vecteur
 * @param b second vecteur
 * @return L'angle absolu a,b
 */
float Vector_angle_fast(Vector a, Vector b);

#endif
