#include "config.h"
#include "game.h"
#include "render_sdl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    srand(RANDOM_SEED);

    bool use_sdl = false;

    if (argc > 1 && strcmp(argv[1], "sdl") == 0) {
        use_sdl = true;
    }

    if (use_sdl) {
        Game_run_SDL();
    } else {
        Game_run_terminal();
    }

    return 0;
}