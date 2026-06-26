#include "input_output.h"
#include "config.h"
#include <stdio.h>

bool save_params(VectorRule *theta, Hyperparameters *hyperparams,
                 float best_gain, char *filename) {
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

    fprintf(file, "center: %f\n", theta->vect[Rules_Center]);
    fprintf(file, "alignment: %f\n", theta->vect[Rules_Alignment]);
    fprintf(file, "pursuit: %f\n", theta->vect[Rules_Pursuit]);
    fprintf(file, "front: %f\n", theta->vect[Rules_zone_Front]);
    fprintf(file, "back: %f\n", theta->vect[Rules_zone_Back]);
    fprintf(file, "left: %f\n", theta->vect[Rules_zone_Left]);
    fprintf(file, "right: %f\n\n", theta->vect[Rules_zone_Right]);
    fprintf(file, "closest shark: %f\n\n", theta->vect[Rules_closest_shark]);

    // Sauvegarde du gain max
    fprintf(file, "# Best gain with this parameters\n");
    fprintf(file, "gain: %f\n", best_gain);

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
            theta->vect[Rules_Center] = value_f;
        } else if (sscanf(line, "alignment: %f", &value_f) == 1) {
            theta->vect[Rules_Alignment] = value_f;
        } else if (sscanf(line, "pursuit: %f", &value_f) == 1) {
            theta->vect[Rules_Pursuit] = value_f;
        } else if (sscanf(line, "front: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Front] = value_f;
        } else if (sscanf(line, "back: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Back] = value_f;
        } else if (sscanf(line, "left: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Left] = value_f;
        } else if (sscanf(line, "right: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Right] = value_f;
        } else if (sscanf(line, "closest shark: %f", &value_f)) {
            theta->vect[Rules_closest_shark] = value_f;
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
    float value_f;

    while (fgets(line, sizeof(line), file)) {
        // Ignorer les commentaires et les lignes vides
        if (line[0] == '#' || line[0] == '\n')
            continue;

        if (sscanf(line, "center: %f", &value_f) == 1) {
            theta->vect[Rules_Center] = value_f;
        } else if (sscanf(line, "alignment: %f", &value_f) == 1) {
            theta->vect[Rules_Alignment] = value_f;
        } else if (sscanf(line, "pursuit: %f", &value_f) == 1) {
            theta->vect[Rules_Pursuit] = value_f;
        } else if (sscanf(line, "front: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Front] = value_f;
        } else if (sscanf(line, "back: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Back] = value_f;
        } else if (sscanf(line, "left: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Left] = value_f;
        } else if (sscanf(line, "right: %f", &value_f) == 1) {
            theta->vect[Rules_zone_Right] = value_f;
        } else if (sscanf(line, "closest shark: %f", &value_f) == 1) {
            theta->vect[Rules_closest_shark] = value_f;
        }
    }

    fclose(file);

    return true;
}

bool logs_generation(VectorRule *theta, int gen_number, float avg_reward,
                     float avg_gain, float avg_iteration,
                     VectorRule theta_diff_STEP_LOG,
                     VectorRule theta_diff_1_step, char *filename) {
    if (!theta || !filename) {
        fprintf(stderr, "Erreur: pointeur NULL dans logs_generation\n");
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
    fprintf(file, "center: %f\n", theta->vect[Rules_Center]);
    fprintf(file, "alignment: %f\n", theta->vect[Rules_Alignment]);
    fprintf(file, "pursuit: %f\n", theta->vect[Rules_Pursuit]);
    fprintf(file, "front: %f\n", theta->vect[Rules_zone_Front]);
    fprintf(file, "back: %f\n", theta->vect[Rules_zone_Back]);
    fprintf(file, "left: %f\n", theta->vect[Rules_zone_Left]);
    fprintf(file, "right: %f\n\n", theta->vect[Rules_zone_Right]);
    fprintf(file, "closest shark: %f\n\n", theta->vect[Rules_closest_shark]);

    fprintf(file, "Moyenne reward: %f\n", avg_reward);
    fprintf(file, "Moyenne gain: %f\n", avg_gain);
    fprintf(file, "Moyenne iteration: %f\n", avg_iteration);
    fprintf(file, "Δθ après %d générations:           ", STEP_LOG);
    VectorRule_fprint(file, theta_diff_STEP_LOG);
    fprintf(file, "\nMoyenne de Δθ de chaque génération: ");
    VectorRule_fprint(file, theta_diff_1_step);
    fprintf(file, "\n\n");

    fclose(file);
    return true;
}

/**
 * @brief charge le gain stocké dans le fichier de paramètres
 * @param gain pointeur vers la variable où stocker le gain
 * @param filename_params nom du fichier de paramètres
 * @return true si le chargement a réussi, false sinon
 */
bool load_gain_params(float *gain, char *filename_params) {
    // Récupération de best gain de l'ancienne run
    FILE *file = fopen(filename_params, "r");
    if (!file) {
        fprintf(stderr, "Erreur: impossible d'ouvrir le fichier %s\n",
                filename_params);
        return false;
    }
    char line[256];
    float value_f;
    while (fgets(line, sizeof(line), file)) {
        // Ignorer les commentaires et les lignes vides
        if (line[0] == '#' || line[0] == '\n')
            continue;
        // Gain max obtenue à la dernière éxecution
        if (sscanf(line, "gain: %f", &value_f) == 1) {
            *gain = value_f;
        }
    }
    return true;
}

/**
 * @brief Parcourt le fichier de log et récupère les paramètres associés au
 * avg_gain le plus fort.
 * @param filename      Nom du fichier de log à lire.
 * @param best_theta    Pointeur vers la structure où stocker le meilleur Theta
 * trouvé.
 * @param best_gen      Pointeur pour récupérer le numéro de la meilleure
 * génération.
 * @param best_gain     Pointeur pour récupérer la valeur du meilleur gain.
 * @return true si la lecture a réussi et qu'un gain a été trouvé, false sinon.
 */
bool get_best_theta(char *filename_logs, VectorRule *best_theta, int *best_gen,
                    float *best_gain) {
    if (!filename_logs || !best_theta || !best_gen || !best_gain) {
        fprintf(stderr, "Erreur: Pointeur NULL passé à get_best_theta\n");
        return false;
    }

    FILE *file = fopen(filename_logs, "r");
    if (!file) {
        fprintf(stderr,
                "Erreur: Impossible d'ouvrir le fichier %s en lecture\n",
                filename_logs);
        return false;
    }

    char line[256];

    // Variables temporaires pour stocker le bloc en cours de lecture
    int current_gen = -1;
    float current_center = 0, current_alignment = 0, current_pursuit = 0,
          current_closest_shark = 0;
    float current_front = 0, current_back = 0, current_left = 0,
          current_right = 0;
    float current_gain = -1.0f;

    // Variables pour suivre si on a trouvé au moins un enregistrement valide
    bool found_any = false;
    float max_gain = -1.0f;

    while (fgets(line, sizeof(line), file)) {

        // 1. Détection du numéro de génération
        if (sscanf(line, "# Génération num: %d", &current_gen) == 1) {
            continue;
        }

        // Theta
        if (sscanf(line, "center: %f", &current_center) == 1)
            continue;
        if (sscanf(line, "alignment: %f", &current_alignment) == 1)
            continue;
        if (sscanf(line, "pursuit: %f", &current_pursuit) == 1)
            continue;
        if (sscanf(line, "front: %f", &current_front) == 1)
            continue;
        if (sscanf(line, "back: %f", &current_back) == 1)
            continue;
        if (sscanf(line, "left: %f", &current_left) == 1)
            continue;
        if (sscanf(line, "right: %f", &current_right) == 1)
            continue;
        if (sscanf(line, "closest shark: %f", &current_closest_shark) == 1)
            continue;

        // Avg_gain
        if (sscanf(line, "Moyenne gain: %f", &current_gain) == 1) {

            if (!found_any || current_gain > max_gain) {
                max_gain = current_gain;
                *best_gen = current_gen;
                *best_gain = current_gain;

                // On sauvegarde le meilleur Theta
                best_theta->vect[Rules_Center] = current_center;
                best_theta->vect[Rules_Alignment] = current_alignment;
                best_theta->vect[Rules_Pursuit] = current_pursuit;
                best_theta->vect[Rules_zone_Front] = current_front;
                best_theta->vect[Rules_zone_Back] = current_back;
                best_theta->vect[Rules_zone_Left] = current_left;
                best_theta->vect[Rules_zone_Right] = current_right;
                best_theta->vect[Rules_closest_shark] = current_closest_shark;

                found_any = true;
            }
        }
    }

    fclose(file);
    return found_any;
}

bool init_logs(char *filename_params, char *filename_logs) {
    if (!filename_logs || !filename_params) {
        fprintf(stderr, "Erreur: pointeur NULL dans init_logs\n");
        return false;
    }

    Hyperparameters hyperparams;
    VectorRule theta_initial;
    float best_gain_initial = 0.0f;

    if (!load_params(&theta_initial, &hyperparams, filename_params)) {
        fprintf(stderr,
                "Erreur: Impossible de charger les paramètres depuis %s\n",
                filename_params);
        return false;
    }
    load_gain_params(&best_gain_initial, filename_params);

    FILE *file = fopen(filename_logs, "a");
    if (!file) {
        fprintf(stderr,
                "Erreur: impossible d'initialiser le fichier de log %s\n",
                filename_logs);
        return false;
    }

    fprintf(file, "# ==========================================\n");
    fprintf(file, "# INITIALISATION DU LOG DE REINFORCE\n");
    fprintf(file, "# ==========================================\n\n");

    fprintf(file, "# Hyperparameters utilisés :\n");
    fprintf(file, "gamma %f\n", hyperparams.gamma);
    fprintf(file, "sigma %f\n", hyperparams.sigma);
    fprintf(file, "alpha %f\n", hyperparams.alpha);
    fprintf(file, "nb_gen %d\n", hyperparams.nb_gen);
    fprintf(file, "nb_game %d\n", hyperparams.nb_game);
    fprintf(file, "nb_occurrence %d\n\n", hyperparams.nb_occurrence);

    fprintf(file, "# Theta de départ :\n");
    fprintf(file, "center: %f\n", theta_initial.vect[Rules_Center]);
    fprintf(file, "alignment: %f\n", theta_initial.vect[Rules_Alignment]);
    fprintf(file, "pursuit: %f\n", theta_initial.vect[Rules_Pursuit]);
    fprintf(file, "front: %f\n", theta_initial.vect[Rules_zone_Front]);
    fprintf(file, "back: %f\n", theta_initial.vect[Rules_zone_Back]);
    fprintf(file, "left: %f\n", theta_initial.vect[Rules_zone_Left]);
    fprintf(file, "right: %f\n\n", theta_initial.vect[Rules_zone_Right]);
    fprintf(file, "closest shark: %f\n\n",
            theta_initial.vect[Rules_closest_shark]);

    fprintf(file, "# Meilleur gain de départ (historique) :\n");
    fprintf(file, "gain_initial: %f\n\n", best_gain_initial);

    fprintf(file, "# ==========================================\n");
    fprintf(file, "# DÉBUT DES LOGS DE GÉNÉRATION\n");
    fprintf(file, "# ==========================================\n\n");

    fclose(file);
    return true;
}

bool end_logs(char *filename_params, char *filename_logs) {
    Hyperparameters hyperparams;
    VectorRule theta_params;

    load_params(&theta_params, &hyperparams, filename_params);
    float gain_params;
    load_gain_params(&gain_params, filename_params);

    VectorRule theta_logs;
    int best_gen_logs = 0;
    float best_gain_logs = -1.0f;

    get_best_theta(filename_logs, &theta_logs, &best_gen_logs, &best_gain_logs);

    if (gain_params < best_gain_logs) {
        save_params(&theta_logs, &hyperparams, best_gain_logs, filename_params);
    }

    printf("====Fin de run====\n");
    printf("\nHyperparameters\n");
    printf("  Gamma :         %f\n", hyperparams.gamma);
    printf("  Sigma :         %f\n", hyperparams.sigma);
    printf("  Alpha :         %f\n", hyperparams.alpha);
    printf("  Nb Gen :        %d\n", hyperparams.nb_gen);
    printf("  Nb Game :       %d\n", hyperparams.nb_game);
    printf("  Nb Occurrence : %d\n", hyperparams.nb_occurrence);

    printf("\nAnciens Paramètres (Fichier params)\n");
    printf("  Gain max :      %f\n", gain_params);
    printf("  Center :        %f\n", theta_params.vect[Rules_Center]);
    printf("  Alignment :     %f\n", theta_params.vect[Rules_Alignment]);
    printf("  Pursuit :       %f\n", theta_params.vect[Rules_Pursuit]);
    printf("  Front :         %f\n", theta_params.vect[Rules_zone_Front]);
    printf("  Back :          %f\n", theta_params.vect[Rules_zone_Back]);
    printf("  Left :          %f\n", theta_params.vect[Rules_zone_Left]);
    printf("  Right :         %f\n", theta_params.vect[Rules_zone_Right]);
    printf(" Closest Shark :        %f\n",
           theta_params.vect[Rules_closest_shark]);

    printf("\nMeilleurs Paramètres Trouvés (Fichier logs)\n");
    printf("  Génération :    %d\n", best_gen_logs);
    printf("  Best Gain :     %f\n", best_gain_logs);
    printf("  Center :        %f\n", theta_logs.vect[Rules_Center]);
    printf("  Alignment :     %f\n", theta_logs.vect[Rules_Alignment]);
    printf("  Pursuit :       %f\n", theta_logs.vect[Rules_Pursuit]);
    printf("  Front :         %f\n", theta_logs.vect[Rules_zone_Front]);
    printf("  Back :          %f\n", theta_logs.vect[Rules_zone_Back]);
    printf("  Left :          %f\n", theta_logs.vect[Rules_zone_Left]);
    printf("  Right :         %f\n", theta_logs.vect[Rules_zone_Right]);
    printf("  Closest Shark : %f\n", theta_logs.vect[Rules_closest_shark]);

    if (gain_params < best_gain_logs) {
        printf("===Theta Modifié===\n");
    } else {
        printf("===Theta Non modifié===\n");
    }
    return true;
}
