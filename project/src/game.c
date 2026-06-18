#include "game.h"
#include "config.h"
#include "fish.h"
#include "mj.h"
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

    game->world = World_init(width, height, nb_fish);
    if (!game->world) {
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

void Game_step(Game *game) {
    World tmp_world;
    tmp_world.width = game->world->width;
    tmp_world.height = game->world->height;
    tmp_world.nb_fish = game->world->nb_fish;

    tmp_world.fishes = calloc(tmp_world.nb_fish, sizeof(Fish));
    if (!tmp_world.fishes) {
        fprintf(stderr, "Erreur malloc dans Game_step (fishes)\n");
        return;
    }

    tmp_world.shark = calloc(1, sizeof(Shark));
    if (!tmp_world.shark) {
        free(tmp_world.fishes);
        fprintf(stderr, "Erreur malloc dans Game_step (shark)\n");
        return;
    }

    UpdateWorld(game->world, &tmp_world);

    free(game->world->fishes);
    // Remplacement par le nouveau
    game->world->fishes = tmp_world.fishes;
    game->world->nb_fish = tmp_world.nb_fish; // si le nombre a changé

    // Pour le requin, on copie la structure (pas d'échange de pointeur)
    //*game->world.shark = *tmp_world.shark;
    // free(tmp_world.shark); // on libère le pointeur temporaire
}

void Game_run() {
    Game game;
    if (!Game_init(&game, WIDTH, HEIGHT, FISH_NB)) {
        fprintf(stderr, "Echec de l'initialisation du jeu.\n");
        return;
    }

    bool quit = false;

    while (!quit) {
        Render_world(&game.display, game.world);

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
                    game.paused = !game.paused;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }

            if (!game.paused) {
                Game_step(&game);
            }

            SDL_Delay(10);
        }
    }
    Game_destroy(&game);
}

void Game_destroy(Game *game) {
    if (!game)
        return;

    World_destroy(game->world);
    Destroy_sdl_display(&game->display);
}
