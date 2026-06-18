#ifndef SHARK_H
#define SHARK_H

#include "config.h"
#include "vector.h"
#include <stdlib.h>

typedef struct {
    Vector pos;
    Vector velocity;
} Shark;

Shark *Shark_createRandom(int width, int height);
int Shark_copy(Shark *shark_dest, Shark *shark_src);
void Shark_destroy(Shark *shark);

#endif
