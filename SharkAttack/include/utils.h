#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

float random_float(float min, float max);
float rand_trsf(float min, float max);
Vector box_muller_standard();

#endif
