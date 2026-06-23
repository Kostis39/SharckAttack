#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

void init_seed(unsigned int set_seed);
float random_float(float min, float max);
int rand_trsf();

Vector box_muller_standard();

#endif
