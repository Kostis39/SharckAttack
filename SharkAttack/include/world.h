#ifndef WORLD_H
#define WORLD_H

#include "collider.h"
#include "config.h"
#include "fish.h"
#include "rules_set.h"
#include "shark.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    Shark *shark;        /**< Pointeur vers le requin présent dans le monde */
    Collider *colliders; /**< Tableau dynamiques d'objet de collision présent
                            dans le monde*/
    int fish_eaten;      /**< Nombre de poissons mangés par le requin */
    RulesSetFish *rules_set_fish;   /**< Ensemble de règles pour les poissons */
    RulesSetShark *rules_set_shark; /**< Ensemble de règles pour le requin */
} World;

World *World_init(int width, int height, int nb_fish, int nb_colliders);
void World_destroy(World *world);
void World_update(World *world, World *world_tmp);

#endif
