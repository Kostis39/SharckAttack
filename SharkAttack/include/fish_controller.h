#ifndef AGENT_FISH_H
#define AGENT_FISH_H

#include "config.h"
#include "fish.h"
#include "shark.h"
#include "theta_set.h"
#include "vector.h"

/** @struct FishPerception
 * @brief Structure représentant la perception qu'a un poisson du monde.
 */
typedef struct {
    Fish self; /**< Le poisson lui-même, pour lequel on calcule la perception */

    Vector collider_separation; /**< Indique le prochain obstacle à proximité*/
    Vector separation;     /**< Vecteur de séparation des voisins proches */
    Vector center_of_mass; /**< Cohesion: Centre de masse des voisins pour la
                              cohésion */
    Vector avg_velocity;   /**< Alignement: Vélocité moyenne des voisins pour
                              l'alignement */
    Vector dist_shark;     /**< Vecteur de distance du requin, si visible */

    bool shark_visible; /**< Indique si le requin est visible par le poisson :
                           true ou non : false */
    Shark shark;

    int width;  /**< Largeur en pixel de notre monde */
    int height; /**< Hauteur en pixel de notre monde */
} FishPerception;

/**
 * @brief calcule l'action du poisson combine linéairement toutes les règles
 * avec leurs poids
 * @param p perception du poisson
 * @param theta poids des règles
 * @param width largeur du monde
 * @param height hauteur du monde
 * @return vecteur vitesse
 */
Vector fish_choose_action(FishPerception *perception, RulesSetFish *rules_set,
                          int width, int height);

#endif
