#ifndef REINFORCE_H
#define REINFORCE_H

#include "config.h"
#include "shark_controller.h"
#include "vector.h"
#include <assert.h>
#include <stdlib.h>

typedef struct {
    Vector mu;
    Vector phi;
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
    Vector gradX; /**< Gradient pour x */
    Vector gradY; /**< Gradient pour y */
} Gradient;

Trajectory *Trajectory_init();
void Trajectory_destroy(Trajectory *trajectory);
void Add_step(Trajectory *trajectory, Vector state, Vector action,
              float reward);

int Need_trajectory_growing(Trajectory *trajectory);

void Trajectory_print(Trajectory *trajectory);

#endif