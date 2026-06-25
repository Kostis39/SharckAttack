#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "config.h"
#include "utils_reinforce.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool save_params(VectorRule *theta, Hyperparameters *hyperparams,
                 float best_gain, char *filename);

bool load_params(VectorRule *theta, Hyperparameters *hyperparams,
                 char *filename);

bool load_theta(VectorRule *theta, char *filename);

bool logs_generation(VectorRule *theta, int gen_number, float avg_reward,
                     float avg_gain, float avg_iteration, char *filename);

bool init_logs(char *filename_params, char *filename_logs);

bool end_logs(char *filename_params, char *filename_logs);

#endif