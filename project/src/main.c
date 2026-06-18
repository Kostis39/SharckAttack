#include "config.h"
#include "render_sdl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    SDLDisplay display;
    if (!Init_sdl_display(&display, "Test SDL", 500, 600)) {
        Destroy_sdl_display(&display);
        return 0;
    }
    World *world = World_init(500, 600, FISH_NB);

    Destroy_sdl_display(&display);
    World_destroy(world);
    return 1;
}