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

    Game_run_SDL();

    // Game_run_terminal();

    return 0;
}