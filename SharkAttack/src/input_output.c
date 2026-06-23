#include "input_output.h"

bool save_params(SharkTheta *theta, Hyperparameters *hyperparams,
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
    fprintf(file, "\n# theta\n");

    fprintf(file, "theta_x");
    fprintf(file, " %f", theta->x.center);
    fprintf(file, " %f", theta->x.alignment);
    fprintf(file, " %f", theta->x.pursuit);
    fprintf(file, "\n");

    fprintf(file, "theta_y");
    fprintf(file, " %f", theta->y.center);
    fprintf(file, " %f", theta->y.alignment);
    fprintf(file, " %f", theta->y.pursuit);
    fprintf(file, "\n");

    fclose(file);
    return true;
}

bool load_params(SharkTheta *theta, Hyperparameters *hyperparams,
                 char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n",
                filename);
        return false;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Ignorer les commentaires et les lignes vides
        if (line[0] == '#' || line[0] == '\n')
            continue;

        float value_f;
        int value_i;

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

        // theta_x
        else if (strncmp(line, "theta_x", 7) == 0) {
            char *ptr = line + 7;

            sscanf(ptr, " %f %f %f", &theta->x.center, &theta->x.alignment,
                   &theta->x.pursuit);
        }

        // theta_y
        else if (strncmp(line, "theta_y", 7) == 0) {
            char *ptr = line + 7;

            sscanf(ptr, " %f %f %f", &theta->y.center, &theta->y.alignment,
                   &theta->y.pursuit);
        }
    }

    fclose(file);

    return true;
}