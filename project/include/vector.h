#ifndef VECTOR_H
#define VECTOR_H

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

Vector Vector_init(); // Initialise à 0 x et y
Vector Vector_add(Vector a, Vector b);
Vector Vector_sub(Vector a, Vector b);
Vector Vector_scale(Vector v, float k);
float Vector_length(Vector v);
float Vector_distance(Vector a, Vector b);

/* Renvoie v de longueur 1 (vecteur nul renvoyé inchangé si v est nul) */
Vector Vector_normalize(Vector v);

/* Tronque v à une longueur max, sans changer sa direction */
Vector Vector_limit(Vector v, float max_length);
float Vector_angle(Vector a, Vector b);

Vector apply_collision_rect(Vector pos, Vector vel, int min_x, int max_x,
                            int min_y, int max_y);
#endif
