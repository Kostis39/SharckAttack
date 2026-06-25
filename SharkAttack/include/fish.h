#ifndef FISH_H
#define FISH_H

#include "config.h"
#include "math.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>
/**
 * @struct Fish
 * @brief Structure représentant un poisson dans le jeu.
 */
typedef struct {
    Vector position; /**< Position du poisson dans le monde */
    Vector velocity; /**< Vecteur direction/vitesse du poisson dans le monde */
    bool
        is_alive; /**< Indique si le poisson est vivant : true ou non : false */

    float radius_separation; /**< Rayon de perception du poisson pour les règles
                              de séparation */
    float radius_alignement; /**< Rayon de perception du poisson pour les règles
                               de alignement */
    float radius_cohesion;   /**< Rayon de perception du poisson pour les règles
                                de cohésion */
    float vision_angle;      /**< Angle de vision du poisson en radians */
} Fish;

/**
 * @brief crée un poisson avec une position et une vitesse aléatoires
 * @param width largeur du monde
 * @param height hauteur du monde
 * @return structure Fish initialisée
 */
Fish Fish_create_random_pos(int width, int height);

/**
 * @brief crée un tableau de poissons avec des positions et vitesses aléatoires
 * @param nb_fish nombre de poissons à créer
 * @param width largeur du monde
 * @param height hauteur du monde
 * @return pointeur vers le tableau alloué dynamiquement
 */
Fish *Fish_create_random_array(int nb_fish, int width, int height);

/**
 * @brief copie un tableau de poissons
 * @param fish_array tableau source
 * @param nb_fish nombre de poissons à copier
 * @return nouveau tableau alloué dynamiquement contenant une copie des données
 */
Fish *Fish_copy_array(Fish *fish_array, int nb_fish);

/**
 * @brief libère la mémoire d'un tableau de poissons
 * @param fish_array tableau à libérer
 */
void Fish_destroy_array(Fish *fish_array);

/**
 * @brief applique une action au poisson met à jour sa vitesse et sa position,
 * puis applique les bornes de vitesse min/max
 * @param fish pointeur vers le poisson
 * @param weighted_velocity vecteur vitesse pondéré
 */
void fish_apply_action(Fish *fish, Vector action);

#endif
