#include "game.h"
#include "config.h"
#include "fish.h"
#include "mj.h"
#include "render_sdl.h"
#include "shark.h"
#include "sound.h"
#include "utils.h"
#include "world.h"
#include <signal.h>
#include <stdlib.h>

static volatile sig_atomic_t terminal_interrupted = 0;

static void Handle_terminal_interrupt(int signum) {
    (void)signum;
    terminal_interrupted = 1;
}

/**
 * @brief initialise le jeu (fenêtre, mondes, paramètres)
 * @param game pointeur vers la structure Game
 * @param width largeur de la fenêtre
 * @param height hauteur de la fenêtre
 * @param nb_fish nombre initial de poissons
 * @param nb_collider nombre d'obstacles
 * @return true si l'initialisation a réussi, false sinon
 */
bool Game_init(Game *game, int width, int height, int nb_fish, int nb_collider,
               int seed_for_worlds) {
    if (!game)
        return false;

    if (!Init_sdl_display(&game->display, "Shark Attack", width, height)) {
        return false;
    }
    game->audio = audio_init();
    VectorRule *shark_theta = SharkTheta_init();
    init_seed(seed_for_worlds);
    game->world1 = World_init(width / 2, height, nb_fish, nb_collider,
                              *shark_theta, 0, NB_OCCURRENCE, true, false);
    init_seed(seed_for_worlds);
    game->world2 = World_init(width / 2, height, nb_fish, nb_collider,
                              *shark_theta, 0, NB_OCCURRENCE, false, false);
    if (!game->world1 || !game->world2) {
        free(shark_theta);
        Destroy_sdl_display(&game->display);
        audio_quit(game->audio);
        return false;
    }
    audio_load(AUDIO_PATH, game->audio);
    audio_load_music(AUDIO_PATH, game->audio);
    audio_play_music(game->audio);

    free(shark_theta);

    game->time = 0;
    game->paused = false;

    return true;
}

/**
 * @brief inverse l'état de pause du jeu
 * @param game pointeur vers la structure Game
 */
void Game_pause(Game *game) {
    if (!game)
        return;
    game->paused = !game->paused;
}

/**
 * @brief lance la boucle de jeu en mode graphique (SDL)
 * @param use_bench true pour exécuter un benchmark
 */
void Game_run_SDL(bool use_bench, int seed_for_worlds) {
    Game game;
    if (!Game_init(&game, WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB,
                   seed_for_worlds)) {
        fprintf(stderr, "Echec de l'initialisation du jeu.\n");
        return;
    }

    bool quit = false;
    int it = 0;
    int intro_start = (int)SDL_GetTicks();
    SDL_Event event;
    while (!quit) {
        Render_two_worlds(&game.display, game.world1, game.world2, game.audio);
        /* Render_world(&game.display, game.world); */
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
        int intro_running = ((int)SDL_GetTicks() - intro_start < 3000);

        if (!game.paused && !intro_running) {
            Game_step(game.world1);
            Game_step(game.world2);
        }
        if (!use_bench) {

            SDL_Delay(10);
        }
    }
    Game_destroy(&game);
}

/**
 * @brief lance la boucle de jeu en mode terminal (sans SDL)
 * affiche les résultats et la trajectoire dans la console
 */
void Game_run_terminal() {
    terminal_interrupted = 0;
    int i = 0;
    signal(SIGINT, Handle_terminal_interrupt);

    VectorRule *shark_theta = SharkTheta_init();
    World *world = World_init(WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB,
                              *shark_theta, 0, NB_OCCURRENCE, false, false);

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
           "Theta : ",
           i, world->nb_fish - world->fish_eaten, world->theta_fish->separation,
           world->theta_fish->alignment, world->theta_fish->cohesion,
           world->theta_fish->shark_avoidance, world->shark->pos.x,
           world->shark->pos.y);
    VectorRule_print(world->theta_shark);
    printf("\n");

    World_destroy(world);
    free(shark_theta);
}

/**
 * @brief libère les ressources allouées par le jeu
 * @param game pointeur vers la structure Game
 */
void Game_destroy(Game *game) {
    if (!game)
        return;
    audio_quit(game->audio);
    World_destroy(game->world1);
    World_destroy(game->world2);
    Destroy_sdl_display(&game->display);
}
