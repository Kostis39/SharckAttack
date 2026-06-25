#ifndef MJ_H
#define MJ_H
#include "config.h"
#include "fish.h"
#include "fish_controller.h"
#include "reinforce.h"
#include "shark.h"
#include "shark_controller.h"
#include "vector.h"
#include "world.h"
/* #include "shark.h" */

/**
 * @brief Initialise la perception d'un poisson
 * @param fish le poisson à initialiser
 * @param world le monde contenant les dimensions
 * @param perception la structure de perception à remplir
 */
void fish_perception_init(Fish *fish, World *world, FishPerception *perception);

/**
 * @brief réalise une itération complète du jeu
 * @param world le monde à mettre à jour
 */
void Game_step(World *world);

/**
 * @brief mise à jour synchrone du monde en calculant les nouvelles positions
 * des agents
 * @param world monde actuel
 * @param tmp_world monde temporaire
 */
void UpdateWorld(World *world, World *tmp_world);

#endif
