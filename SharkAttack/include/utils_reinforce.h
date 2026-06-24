#ifndef UTILS_REINFORCE_H
#define UTILS_REINFORCE_H

#include "theta_set.h"
#include "vector.h"

typedef struct StepTrajectory {
    SharkPhi phi;
    Vector action;
    float reward;
} StepTrajectory;

typedef struct {
    StepTrajectory *steps;
    int length;       /**< Longueur actuelle */
    int lenght_alloc; /**< Taille en mémoire actuelle prise par le tableau
                         steps*/
    int capacity;     /**< Capicité maximale d'une trajectoire */
} Trajectory;

typedef struct {
    VectorRule x; /**< Gradient pour x */
    VectorRule y; /**< Gradient pour y */
} Gradient;

typedef struct {
    int nb_gen;
    int nb_game;
    int nb_occurrence;
    float alpha;
    float gamma;
    float sigma;
} Hyperparameters;

typedef struct {
    SharkTheta theta;
    Hyperparameters hyperparameters;
    Gradient *grad_target;

} WorkerArgs;

#endif