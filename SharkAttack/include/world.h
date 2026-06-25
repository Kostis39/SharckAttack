#ifndef WORLD_H
#define WORLD_H

#include "collider.h"
#include "config.h"
#include "fish.h"
#include "reinforce.h"
#include "shark.h"
#include "theta_set.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @struct World
 * @brief Structure représentant le monde du jeu, contenant les poissons et le
 * requin.
 */
typedef struct {
    int width;  /**< Largeur en pixel de notre monde. */
    int height; /**< Hauteur en pixel de notre monde. */

    Fish *fishes; /**< Tableau dynamique de poissons présents dans le monde. */
    int nb_fish;  /**< Nombre de poissons présents dans le monde. */

    Shark *shark;        /**< Pointeur vers le requin présent dans le monde. */
    Collider *colliders; /**< Tableau dynamiques d'objet de collision présent
                            dans le monde. */
    int nb_colliders;    /**< Le nombre de colliders dans notre monde. */
    int fish_eaten;      /**< Nombre de poissons mangés par le requin. */
    RulesSetFish *theta_fish; /**< Ensemble de règles pour les poissons. */
    VectorRule theta_shark;   /**< Ensemble de règles pour le requin. */
    Trajectory *trajectory; /**< Tableau stockant chaque étape du jeu, stocke la
                               trajectoire. */
    float sigma;            /**< Valeur du sigma pour notre tirage aléatoire. */
    int nb_occurrence;      /**< Nombre d'occurence maximale d'une partie. */

    bool learn; /**< Nécessaire pour savoir si on lance le monde en mode
                   apprentissage ou non. */
} World;

/**
 * @brief initialise un monde avec des paramètres
 * @param width largeur du monde en pixels
 * @param height hauteur du monde en pixels
 * @param nb_fish nombre de poissons
 * @param nb_colliders nombre d'obstacles
 * @param theta_shark paramètres de la politique du requin
 * @param sigma écart-type pour l'exploration (pour l'apprentissage)
 * @param nb_occurrence nombre maximum de pas par épisode (pour l'apprentissage)
 * @param is_player true si le requin est contrôlé par le joueur
 * @param learn true si en mode apprentissage
 * @return pointeur vers le monde alloué dynamiquement
 */
World *World_init(int width, int height, int nb_fish, int nb_colliders,
                  VectorRule theta_shark, float sigma, int nb_occurrence,
                  bool is_player, bool learn);

/**
 * @brief libère un monde et tout ses éléments
 * @param world pointeur vers le monde à détruire
 */
void World_destroy(World *world);

/**
 * @brief crée un monde temporaire à partir d'un monde existant
 * @param world pointeur vers le monde source
 * @param tmp_world pointeur vers le monde temporaire à remplir
 * @return true si la création a réussi, false sinon
 */
bool World_create_tmp(const World *world, World *tmp_world);

/**
 * @brief échange les données entre le monde réel et le monde temporaire
 * @param world pointeur vers le monde réel à modifier
 * @param tmp_world pointeur vers le monde temporaire qui sera détruit après
 * l'échange
 */
void World_swap_data(World *world, World *tmp_world);

#endif
