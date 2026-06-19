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
    srand(time(NULL));

    Game_run();
    return 0;
}