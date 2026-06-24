#include "input_output.h"

bool save_params(VectorRule *theta, Hyperparameters *hyperparams,
                 char *filename) {
    if (!theta || !hyperparams || !filename) {
        fprintf(stderr, "Erreur: pointeur NULL dans save_params\n");
        return false;
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n",
                filename);
        return false;
    }

    // Sauvegarde des hyperparamètres
    fprintf(file, "# Hyperparameters\n");
    fprintf(file, "gamma %f\n", hyperparams->gamma);
    fprintf(file, "sigma %f\n", hyperparams->sigma);
    fprintf(file, "alpha %f\n", hyperparams->alpha);
    fprintf(file, "nb_gen %d\n", hyperparams->nb_gen);
    fprintf(file, "nb_game %d\n", hyperparams->nb_game);
    fprintf(file, "nb_occurrence %d\n", hyperparams->nb_occurrence);

    // Sauvegarde de theta
    fprintf(file, "\n# Theta\n");

    fprintf(file, "center: %f\n", theta->center);
    fprintf(file, "alignment: %f\n", theta->alignment);
    fprintf(file, "pursuit: %f\n", theta->pursuit);

    fclose(file);
    return true;
}

bool load_params(VectorRule *theta, Hyperparameters *hyperparams,
                 char *filename) {
    if (!theta || !hyperparams || !filename) {
        fprintf(stderr, "Erreur: pointeur NULL dans load_params\n");
        return false;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n",
                filename);
        return false;
    }

    char line[256];
    float value_f;
    int value_i;

    while (fgets(line, sizeof(line), file)) {
        // Ignorer les commentaires et les lignes vides
        if (line[0] == '#' || line[0] == '\n')
            continue;

        // Hyperparamètres
        if (sscanf(line, "gamma %f", &value_f) == 1) {
            hyperparams->gamma = value_f;
        } else if (sscanf(line, "sigma %f", &value_f) == 1) {
            hyperparams->sigma = value_f;
        } else if (sscanf(line, "alpha %f", &value_f) == 1) {
            hyperparams->alpha = value_f;
        } else if (sscanf(line, "nb_gen %d", &value_i) == 1) {
            hyperparams->nb_gen = value_i;
        } else if (sscanf(line, "nb_game %d", &value_i) == 1) {
            hyperparams->nb_game = value_i;
        } else if (sscanf(line, "nb_occurrence %d", &value_i) == 1) {
            hyperparams->nb_occurrence = value_i;
        }

        // Theta
        else if (sscanf(line, "center: %f", &value_f) == 1) {
            theta->center = value_f;
        } else if (sscanf(line, "alignment: %f", &value_f) == 1) {
            theta->alignment = value_f;
        } else if (sscanf(line, "pursuit: %f", &value_f) == 1) {
            theta->pursuit = value_f;
        }
    }

    fclose(file);

    return true;
}

bool load_theta(VectorRule *theta, char *filename) {
    if (!theta || !filename) {
        fprintf(stderr, "Erreur: pointeur NULL dans load_theta\n");
        return false;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n",
                filename);
        return false;
    }

    char line[256];
    float value_t;

    while (fgets(line, sizeof(line), file)) {
        // Ignorer les commentaires et les lignes vides
        if (line[0] == '#' || line[0] == '\n')
            continue;
        if (sscanf(line, "center: %f", &value_t) == 1) {
            theta->center = value_t;
        } else if (sscanf(line, "alignment: %f", &value_t) == 1) {
            theta->alignment = value_t;
        } else if (sscanf(line, "pursuit: %f", &value_t) == 1) {
            theta->pursuit = value_t;
        }
    }

    fclose(file);

    return true;
}

bool logs_generation(VectorRule *theta, int gen_number, float avg_reward,
                     float avg_gain, char *filename) {
    if (!theta || !filename) {
        fprintf(stderr, "Erreur: pointeur NULL dans load_theta\n");
        return false;
    }
    FILE *file = fopen(filename, "a");
    if (!file) {
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n",
                filename);
        return false;
    }

    // Sauvegarde de theta
    fprintf(file, "# Génération num: %d \n", gen_number);

    fprintf(file, "Theta\n");
    fprintf(file, "center: %f\n", theta->center);
    fprintf(file, "alignment: %f\n", theta->alignment);
    fprintf(file, "pursuit: %f\n\n", theta->pursuit);
    fprintf(file, "Avg reward: %f\n", avg_reward);
    fprintf(file, "Avg gain: %f\n\n", avg_gain);

    fclose(file);
    return true;
}