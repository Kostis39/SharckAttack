#ifndef GAME_H
#define GAME_H

#include "render_sdl.h"
#include "world.h"

/**
 * @struct Game
 * @brief Structure représentant l'état du jeu.
 */
typedef struct {
    World *world; /**< Le monde du jeu, contenant les poissons et le requin */
    SDLDisplay display; /**< L'affichage SDL du jeu */

    float time;
    bool paused; /**< Indique si le jeu est en pause : true ou non : false */
} Game;

bool Game_init(Game *game, int width, int height, int nb_fish,
               int nb_colliders);

void Game_pause(Game *game);

void Game_run_SDL();
void Game_run_terminal();

void Game_destroy(Game *game);

#endif
