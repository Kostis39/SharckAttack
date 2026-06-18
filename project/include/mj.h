#ifndef MAITRE_JEU_H
#define MAITRE_JEU_H

#include "render_sdl.h"
#include "world.h"
#include <stdbool.h>

typedef struct {
    World world;
    // Display display;

    float time;
    bool paused;

    int fish_eaten;
} Game;
;

bool game_init(Game *game, int width, int height, int nb_fish);

void game_step(Game *game);

void game_handle_collisions(Game *game);

void game_pause(Game *game);

void game_end(Game *game);

void game_run(Game *game);

void game_destroy(Game *game);

#endif