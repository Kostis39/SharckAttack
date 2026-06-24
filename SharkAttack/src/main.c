#include "config.h"
#include "game.h"
#include "input_output.h"
#include "render_sdl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    bool use_term = false;
    bool learn = false;
    bool benchmark_mode = false;

    if (argc > 1 && strcmp(argv[1], "term") == 0) {
        use_term = true;
    } else if (argc > 1 && strcmp(argv[1], "-b") == 0) {
        benchmark_mode = true;
    } else if (argc > 1 && strcmp(argv[1], "learn") == 0) {
        learn = true;
    }

    if (use_term) {
        Game_run_terminal();
    } else if (learn) {
        VectorRule theta;
        Hyperparameters hyperparameters;

        if (!load_params(&theta, &hyperparameters, "params.txt")) {
            fprintf(stderr, "Erreur: échec du chargement des paramètres\n");
            return 1;
        }

        printf("=== Paramètres chargés ===\n");
        printf("gamma: %.3f\n", hyperparameters.gamma);
        printf("sigma: %.3f\n", hyperparameters.sigma);
        printf("alpha: %.3f\n", hyperparameters.alpha);
        printf("nb_gen: %d\n", hyperparameters.nb_gen);
        printf("nb_game: %d\n", hyperparameters.nb_game);
        printf("nb_occurrence: %d\n", hyperparameters.nb_occurrence);

        printf("\n=== Theta ===\n");
        printf("theta = (%f, %f, %f)\n", theta.center, theta.alignment,
               theta.pursuit);
        printf("\n");

        Reinforce_learning(&theta, hyperparameters);

        // sauvegarder theta quand l'entraînement est terminé
        if (!save_params(&theta, &hyperparameters, "new_params.txt")) {
            fprintf(stderr, "Erreur: échec de la sauvegarde\n");
        }
    } else {
        Game_run_SDL(benchmark_mode);
    }

    return 0;
}
