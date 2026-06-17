#ifndef AGENT_H
#define AGENT_H

#include "world.h"
#include <stdio.h>

int GetNbNeighbors(Cell **c);
bool NewState(Cell *c, int nbNeightbors);

#endif
