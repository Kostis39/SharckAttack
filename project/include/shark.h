#ifndef SHARK_H
#define SHARK_H

#include "config.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    Vector pos;
    Vector velocity;
    bool player; // true le requin est un joueur, false un bot
} Shark;

Shark *Shark_createRandom(int width, int height);
int Shark_copy(Shark *shark_dest, Shark *shark_src);
void Shark_destroy(Shark *shark);

bool Is_player(Shark *shark);

#endif
