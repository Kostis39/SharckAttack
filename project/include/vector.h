#ifndef VECTOR_H
#define VECTOR_H

#include "config.h"

typedef struct {
    float x;
    float y;
} Vector;

Vector VectorInit(); // Initialise à 0 x et y
Vector VectorAdd(Vector a, Vector b);
Vector VectorSub(Vector a, Vector b);
Vector VectorScale(Vector v, float k);
float VectorLength(Vector v);
float VectorDistance(Vector a, Vector b);

/* Renvoie v de longueur 1 (vecteur nul renvoyé inchangé si v est nul) */
Vector VectorNormalize(Vector v);

/* Tronque v à une longueur max, sans changer sa direction */
Vector VectorLimit(Vector v, float max_length);

#endif