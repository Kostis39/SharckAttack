#ifndef REINFORCE_H
#define REINFORCE_H

#include "config.h"
#include "input_output.h"
#include "shark.h"
#include "shark_controller.h"
#include "utils_reinforce.h"
#include <assert.h>
#include <stdlib.h>

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

Gradient Generate_gradient(VectorRule theta, Hyperparameters hyperparameters);

void Reinforce_learning(VectorRule *theta, Hyperparameters hyperparameters);
#endif
