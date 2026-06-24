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

    VectorRule *shark_theta = SharkTheta_init();

    game->world = World_init(width, height, nb_fish, nb_collider, *shark_theta,
                             0, NB_OCCURRENCE, false);
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

void Game_run_SDL(bool use_bench) {
    Game game;
    if (!Game_init(&game, WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB)) {
        fprintf(stderr, "Echec de l'initialisation du jeu.\n");
        return;
    }

    bool quit = false;
    int it = 0;
    SDL_Event event;
    while (!quit) {
        Render_world(&game.display, game.world);
        if (use_bench) {
            it++;
            if (it > BENCHMARK_ITERATIONS) {
                quit = true;
            }
        }
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
        if (!use_bench) {

            SDL_Delay(10);
        }
    }
    Game_destroy(&game);
}

void Game_run_terminal() {
    terminal_interrupted = 0;
    int i = 0;
    signal(SIGINT, Handle_terminal_interrupt);

    VectorRule *shark_theta = SharkTheta_init();
    World *world = World_init(WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB,
                              *shark_theta, 0, NB_OCCURRENCE, false);

    for (i = 0; (world->nb_fish - world->fish_eaten != 0) &&
                i < world->nb_occurrence && !terminal_interrupted;
         i++) {
        Game_step(world);
        printf("%d %d\n", i, world->fish_eaten);
    }

    if (terminal_interrupted) {
        printf("Boucle interrompue par l'utilisateur.\n");
    }
    Trajectory_print(world->trajectory);
    printf("Iteration: %d\n"
           "Fish : Remaining fishes: %d Separation: %f Alignement: %f "
           "Cohesion: %f Shark "
           "Avoidance: %f\n"
           "Shark : Position: (%f, %f)\n"
           "Theta : (Center:  %f, Alignement: %f, Pursuit: %f)\n",
           i, world->nb_fish - world->fish_eaten, world->theta_fish->separation,
           world->theta_fish->alignment, world->theta_fish->cohesion,
           world->theta_fish->shark_avoidance, world->shark->pos.x,
           world->shark->pos.y, world->theta_shark.center,
           world->theta_shark.alignment, world->theta_shark.pursuit);
    World_destroy(world);
    free(shark_theta);
}

void Game_destroy(Game *game) {
    if (!game)
        return;

    World_destroy(game->world);
    Destroy_sdl_display(&game->display);
}
