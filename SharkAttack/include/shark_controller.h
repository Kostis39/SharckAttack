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
    Shark self;

    Fish closest_fish;
    bool has_prey;

    Vector center_of_mass;
    Vector avg_velocity;
    bool has_prey_visible;

    int width;
    int height;

    // perception par zone en fonction de la direction du requin (avant,
    // arrière, gauche, droite)
    int zone_count[Count];
    Vector zone_center_of_mass[Count];
    Vector zone_avg_velocity[Count];
    bool zone_has_prey_visible[Count];
} SharkPerception;

Vector shark_choose_action(SharkPerception *perception, VectorRule theta,
                           StepTrajectory *step_trajectory, float sigma);

#endif
