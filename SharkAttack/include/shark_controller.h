#ifndef AGENT_SHARK_H
#define AGENT_SHARK_H

#include "fish.h"
#include "shark.h"
#include "theta_set.h"
#include "utils.h"
#include "vector.h"

typedef struct {
    Shark self;

    Fish closest_fish;
    bool has_prey;

    Vector center_of_mass;
    Vector avg_velocity;
    bool has_prey_visible;

    int width;
    int height;
} SharkPerception;

/**
 * @brief Structure représentant le résultat des règles, nommée phi dans le
 * cours.
 *
 */
typedef struct {
    Vector center;    /**< Vecteur résultat de la règle center */
    Vector alignment; /**< Vecteur résultat de la règle alignement */
    Vector pursuit;   /**< Vecteur résultat de la règle pursuit */
} SharkPhi;

Vector shark_choose_action(SharkPerception *perception, SharkTheta *theta);

#endif
