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

/**
 * @brief petite fonction auxiliaire pour déterminer un float dans une
 * range*/
float random_float(float min, float max);

/**
 * @brief Fonction thread safe de randomise.
 *
 * @return int Le nombre aléatoire tiré.
 */
int rand_trsf();

/**
 * @brief Fonction permettant de tirer une variable aléatoire selon une loi
 * gaussienne.
 *
 * @return Vector Vecteur résultat du tirage aléatoire.
 */
Vector box_muller_standard();

#endif
