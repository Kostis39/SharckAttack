#ifndef SHARK_CONTROLLER_H
#define SHARK_CONTROLLER_H

#include "fish.h"
#include "reinforce.h"
#include "shark.h"
#include "theta_set.h"
#include "utils.h"
#include "vector.h"

typedef struct StepTrajectory StepTrajectory;
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

Vector shark_choose_action(SharkPerception *perception, SharkTheta theta,
                           StepTrajectory *step_trajectory);

#endif
