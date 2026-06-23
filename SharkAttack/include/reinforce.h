#ifndef REINFORCE_H
#define REINFORCE_H

#include "config.h"
#include "vector.h"
#include <stdlib.h>

typedef struct {
    Vector state;
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

Trajectory *Trajectory_init();
void Trajectory_destroy(Trajectory *trajectory);
void Add_step(Trajectory *trajectory, Vector state, Vector action,
              float reward);

int Need_trajectory_growing(Trajectory *trajectory);

#endif