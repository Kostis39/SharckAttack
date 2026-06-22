#include "config.h"
#include "game.h"
#include "render_sdl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    srand(RANDOM_SEED);

    bool use_term = false;

    if (argc > 1 && strcmp(argv[1], "term") == 0) {
        use_term = true;
    }

    if (use_term) {
        Game_run_terminal();
    } else {
        Game_run_SDL();
    }

    return 0;
}