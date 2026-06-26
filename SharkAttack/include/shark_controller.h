#ifndef SHARK_CONTROLLER_H
#define SHARK_CONTROLLER_H

#include "fish.h"
#include "reinforce.h"
#include "shark.h"
#include "theta_set.h"
#include "utils.h"
#include "vector.h"

typedef struct StepTrajectory StepTrajectory;

typedef enum { Front = 0, Back, Left, Right, Count } ZoneDirection;

typedef struct {
    Shark self; /**< La perception de lui même. */

    Fish closest_fish; /**< Le poisson le plus proche du requin. */
    bool has_prey; /**< Booléen permet de savoir si un poisson est dans le champ
                      de vision direct du requin. */

    Vector center_of_mass; /**< Centre de masse de tout les poissons dans le
                              champ de vision du requin. */
    Vector avg_velocity;   /**< La velocity moyenne de tout les poissons dans le
                              champ de vision du requin. */
    bool has_prey_visible;

    int width;  /** Largeur de l'écran. */
    int height; /** Hauteur de l'écran. */

    // perception par zone en fonction de la direction du requin (avant,
    // arrière, gauche, droite)
    int zone_count[Count];
    Vector zone_center_of_mass[Count];
    Vector zone_avg_velocity[Count];
    bool zone_has_prey_visible[Count];

    Vector closest_shark; /**< Position du plus proche requin. */

    int nb_fish_remaining;
} SharkPerception;

/**
 * @brief choisit l'action du requin (joueur ou bot)
 * en mode joueur : suit la souris
 * en mode bot : combine centre, alignement et poursuite selon une politique
 * gaussienne
 * @param perception perception du requin
 * @param theta poids des règles
 * @param step_trajectory pointeur vers le pas de trajectoire à remplir (si
 * apprentissage)
 * @param sigma écart-type pour l'exploration
 * @return vecteur action
 */
Vector shark_choose_action(SharkPerception *perception, VectorRule theta,
                           StepTrajectory *step_trajectory, float sigma);

#endif
