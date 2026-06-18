#ifndef MAITRE_JEU_H
#define MAITRE_JEU_H

#include "render_sdl.h"
#include "world.h"

typedef struct {
    World world;
    SDLDisplay display;

    float time;
    bool paused;

    int fish_eaten;
} Game;

bool Game_init(Game *game, int width, int height, int nb_fish);

void Game_step(Game *game);

void Game_handle_collisions(Game *game);

void Game_pause(Game *game);

void Game_end(Game *game);

void Game_run(Game *game);

void Game_destroy(Game *game);

#endif