#ifndef REINFORCE_H
#define REINFORCE_H

#include "config.h"
#include "shark.h"
#include "shark_controller.h"
#include "theta_set.h"
#include "vector.h"
#include <assert.h>
#include <stdlib.h>

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
    float alpha;
    float gamma;
    float sigma;
} Hyperparameters;

Trajectory *Trajectory_init();
void Trajectory_destroy(Trajectory *trajectory);
void Add_step(Trajectory *trajectory, SharkPhi phi, Vector action,
              float reward);

int Need_trajectory_growing(Trajectory *trajectory);

void Trajectory_print(Trajectory *trajectory);
void StepTrajectory_print(StepTrajectory step);

void New_step(StepTrajectory *step, SharkPhi phi, Vector action, float reward);

void Step_update(StepTrajectory *step, SharkPhi phi, Vector action,
                 float reward);

Gradient Gradient_zero();

Gradient Generate_gradient(SharkTheta theta, Hyperparameters hyperparameters);
#endif
