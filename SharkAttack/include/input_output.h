#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "config.h"
#include "reinforce.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool save_params(SharkTheta *theta, Hyperparameters *hyperparams,
                 char *filename);

bool load_params(SharkTheta *theta, Hyperparameters *hyperparams,
                 char *filename);

bool load_theta(SharkTheta *theta, char *filename);

#endif