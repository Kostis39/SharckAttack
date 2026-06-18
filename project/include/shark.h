#ifndef SHARK_H
#define SHARK_H

#include "config.h"
#include "fish.h"
#include "vector.h"

typedef struct {
    Vector pos;
    Vector velocity;
} Shark;

Shark *Shark_createRandom(int width, int height);
void Shark_destroy(Shark *shark);

#endif