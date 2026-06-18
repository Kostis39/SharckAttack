#ifndef SHARK_H
#define SHARK_H

#include "config.h"
#include "fish.h"
#include "vector.h"

typedef struct {
    Vector pos;
    Vector velocity;
} Shark;

Shark *SharkCreate(Vector pos); // Créer un requin avec une position en parmètre
                                // et une vitesse aléatoire

#endif