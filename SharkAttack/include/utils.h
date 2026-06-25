#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief initialise le générateur pseudo aléatoire avec la seed set_seed et la
 * retourne
 * @param set_seed seed cible
 * @return la seed initialisée
 * */
int init_seed(unsigned int set_seed);
float random_float(float min, float max);
int rand_trsf();

Vector box_muller_standard();

#endif
