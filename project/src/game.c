#include "game.h"
#include "fish.h"
#include "render_sdl.h"
#include "shark.h"
#include "world.h"
#include <stdlib.h>

bool Game_init(Game *game, int width, int height, int nb_fish) {
    if (!game)
        return false;

    if (!Init_sdl_display(&game->display, "Shark Attack", width, height)) {
        return false;
    }

    if (!World_init(width, height, nb_fish)) {
        Destroy_sdl_display(&game->display);
        return false;
    }

    game->time = 0;
    game->paused = false;
    game->fish_eaten = 0;

    return true;
}

void Game_pause(Game *game) {
    if (!game)
        return;
    game->paused = !game->paused;
}