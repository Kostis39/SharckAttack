#ifndef SHARK_H
#define SHARK_H

#include "vector.h"
#include <stdlib.h>

typedef struct {
    Vector pos;
    Vector velocity;
} Shark;

Shark *Shark_createRandom(int width, int height);
void Shark_destroy(Shark *shark);

#endif
