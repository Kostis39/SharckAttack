#ifndef VECTOR_H
#define VECTOR_H

/* #include "config.h" */

typedef struct {
    float x;
    float y;
} Vector;

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

#endif
