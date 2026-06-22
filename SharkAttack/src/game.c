#include "game.h"
#include "config.h"
#include "fish.h"
#include "mj.h"
#include "render_sdl.h"
#include "shark.h"
#include "world.h"
#include <signal.h>
#include <stdlib.h>

static volatile sig_atomic_t terminal_interrupted = 0;

static void Handle_terminal_interrupt(int signum) {
    (void)signum;
    terminal_interrupted = 1;
}

bool Game_init(Game *game, int width, int height, int nb_fish,
               int nb_collider) {
    if (!game)
        return false;

    if (!Init_sdl_display(&game->display, "Shark Attack", width, height)) {
        return false;
    }

    game->world = World_init(width, height, nb_fish, nb_collider);
    if (!game->world) {
        Destroy_sdl_display(&game->display);
        return false;
    }

    game->time = 0;
    game->paused = false;

    return true;
}

void Game_pause(Game *game) {
    if (!game)
        return;
    game->paused = !game->paused;
}

void Game_run_SDL() {
    Game game;
    if (!Game_init(&game, WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB)) {
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
        }

        if (!game.paused) {
            Game_step(game.world);
        }

        SDL_Delay(10);
    }
    Game_destroy(&game);
}

void Game_run_terminal() {
    terminal_interrupted = 0;
    int i = 0;
    signal(SIGINT, Handle_terminal_interrupt);

    World *world = World_init(WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB);
    printf("Iteration: %d\nFish : Number: %d Separation: %f Alignement: %f "
           "Cohesion: %f Shark "
           "Avoidance: %f\n",
           i, world->nb_fish, world->rules_set_fish->separation,
           world->rules_set_fish->alignment, world->rules_set_fish->cohesion,
           world->rules_set_fish->shark_avoidance);

    for (i = 0; i < NB_OCCURRENCE && !terminal_interrupted; i++) {
        Game_step(world);
        printf("Iteration: %d\nFish : Number: %d Separation: %f Alignement: %f "
               "Cohesion: %f Shark "
               "Avoidance: %f\n",
               i, world->nb_fish, world->rules_set_fish->separation,
               world->rules_set_fish->alignment,
               world->rules_set_fish->cohesion,
               world->rules_set_fish->shark_avoidance);
    }

    if (terminal_interrupted) {
        printf("Boucle interrompue par l'utilisateur.\n");
    }

    printf("Iteration: %d\nFish : Number: %d Separation: %f Alignement: %f "
           "Cohesion: %f Shark "
           "Avoidance: %f\n",
           i, world->nb_fish, world->rules_set_fish->separation,
           world->rules_set_fish->alignment, world->rules_set_fish->cohesion,
           world->rules_set_fish->shark_avoidance);
    World_destroy(world);
}

void Game_destroy(Game *game) {
    if (!game)
        return;

    World_destroy(game->world);
    Destroy_sdl_display(&game->display);
}
