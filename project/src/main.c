#include "config.h"
#include "render_sdl.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    SDLDisplay display;
    if (!Init_sdl_display(&display, "Test SDL", 400, 400)) {
        Destroy_sdl_display(&display);
        return 0;
    }
    World *world = World_init(400, 400, FISH_NB);
    if (world == NULL) {
    Destroy_sdl_display(&display);
    return 0;
}

bool running = true;

while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
        }
    }

    Draw_world(&display, world->fishes, world->nb_fish, world->shark);

    SDL_Delay(16);
}

    Destroy_sdl_display(&display);
    World_destroy(world);
    return 1;
}