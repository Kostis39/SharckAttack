#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

float random_float(float min, float max);
void box_muller_standard(Vector *vect);

#endif