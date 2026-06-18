#ifndef SHARK_H
#define SHARK_H

#include "config.h"
#include "fish.h"
#include "vector.h"

typedef struct {
    Vector pos;
    Vector velocity;
} Shark;

Shark *SharkCreateRandom(int width, int height);
void SharkDestroy(Shark *shark);

#endif