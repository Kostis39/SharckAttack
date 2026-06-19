#ifndef WORLD_H
#define WORLD_H

#include "fish.h"
#include "shark.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @struct World
 * @brief Structure représentant le monde du jeu, contenant les poissons et le
 * requin.
 */
typedef struct {
    int width;  /**< Largeur en pixel de notre monde */
    int height; /**< Hauteur en pixel de notre monde */

    Fish *fishes; /**< Tableau dynamique de poissons présents dans le monde */
    int nb_fish;  /**< Nombre de poissons présents dans le monde */

    Shark *shark; /**< Pointeur vers le requin présent dans le monde */
} World;

World *World_init(int width, int height, int nb_fish);
void World_destroy(World *world);
void World_update(World *world, World *world_tmp);

#endif
