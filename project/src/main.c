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

<<<<<<< HEAD
    SDLDisplay display;
    if (!Init_sdl_display(&display, "Test SDL", 500, 400)) {
        Destroy_sdl_display(&display);
        return 0;
    }
    World *world = World_init(500, 400, FISH_NB);
    if (world == NULL) {
    Destroy_sdl_display(&display);
=======
    Game_run();
>>>>>>> 948a9bd731e158b4f4ac20e70dfa234a1e7af466
    return 0;
}