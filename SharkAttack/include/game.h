#ifndef GAME_H
#define GAME_H

#include "render_sdl.h"
#include "world.h"

/**
 * @struct Game
 * @brief Structure représentant l'état du jeu.
 */
typedef struct {
    World *world1; /**< Le monde du jeu, contenant les poissons et le requin */
    World *world2;
    SDLDisplay display; /**< L'affichage SDL du jeu */

    float time;
    bool paused; /**< Indique si le jeu est en pause : true ou non : false */
} Game;

/**
 * @brief initialise le jeu (fenêtre, mondes, paramètres)
 * @param game pointeur vers la structure Game
 * @param width largeur de la fenêtre
 * @param height hauteur de la fenêtre
 * @param nb_fish nombre initial de poissons
 * @param nb_collider nombre d'obstacles
 * @return true si l'initialisation a réussi, false sinon
 */
bool Game_init(Game *game, int width, int height, int nb_fish, int nb_colliders,
               int seed_for_worlds);

/**
 * @brief inverse l'état de pause du jeu
 * @param game pointeur vers la structure Game
 */
void Game_pause(Game *game);

/**
 * @brief lance la boucle de jeu en mode graphique (SDL)
 * @param use_bench true pour exécuter un benchmark
 */
void Game_run_SDL(bool benchmark_mode, int seed_for_worlds);

/**
 * @brief lance la boucle de jeu en mode terminal (sans SDL)
 * affiche les résultats et la trajectoire dans la console
 */
void Game_run_terminal();

/**
 * @brief libère les ressources allouées par le jeu
 * @param game pointeur vers la structure Game
 */
void Game_destroy(Game *game);

#endif
