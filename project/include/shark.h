#ifndef SHARK_H
#define SHARK_H

#include "vector.h"
typedef struct {
    Vector pos;
    Vector velocity;
} Shark;

Shark *Shark_create(Vector pos); // Créer un requin avec une position en
                                 // parmètre et une vitesse aléatoire

// Vector shark_choose_action(Shark *shark, SharkPerception *perception)

#endif
