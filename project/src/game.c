#include "game.h"
#include "config.h"
#include "fish.h"
#include "render_sdl.h"
#include "shark.h"
#include "world.h"
#include <stdlib.h>
/*
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

void Game_run(Game *game) {
    Game game;
    if (!Game_init(&game, WIDTH, HEIGHT, FISH_NB)) {
        fprintf(stderr, "Echec de l'initialisation du jeu.\n");
        return;
    }

    bool quit = false;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_SPACE:
                    game->paused = !game->paused;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }

            if (!game->paused) {
                Game_step(game);
            }

            Render_world(&game->display, &game->world);

            SDL_Delay(10);
        }
    }
    Game_destroy(game);
}

void Game_destroy(Game *game) {
    if (!game)
        return;

    World_destroy(&game->world);

    Destroy_sdl_display(&game->display);
}
 */