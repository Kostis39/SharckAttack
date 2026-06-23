#ifndef REINFORCE_H
#define REINFORCE_H

#include "config.h"
#include "vector.h"
#include <assert.h>
#include <stdlib.h>

typedef struct {
    Vector state;
    Vector action;
    float reward;
} StepTrajectory;

void New_step(StepTrajectory *step, Vector state, Vector action, float reward);

#endif