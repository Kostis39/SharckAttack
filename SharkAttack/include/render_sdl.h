#ifndef RENDER_SDL_H
#define RENDER_SDL_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "collider.h"
#include "fish.h"
#include "shark.h"
#include "sound.h"
#include "world.h"

/**
 * @struct SDLDisplay
 * @brief Structure représentant l'affichage SDL du jeu.
 */
typedef struct {
    SDL_Window *window;      /**< Pointeur vers la fenêtre SDL */
    SDL_Renderer *renderer;  /**< Pointeur vers le rendu SDL */
    SDL_Texture *left_scene; /**< Texture du monde gauche (Render_two_worlds) */
    SDL_Texture *right_scene; /**< Texture du monde droit (Render_two_worlds) */
} SDLDisplay;

/**
 * @brief initialisation de sdl
 *
 * @param display struct contenant la fenetre et le renderer
 * @param title titre de la fenetre
 * @param width largeur
 * @param height hauteur
 * @return true si init est reussi
 * @return false
 */
bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height);

/**
 * @brief cette fct libere le renderer, detruit la fenetre puis ferme sdl
 *
 * @param display struct sdl a detruire
 */
void Destroy_sdl_display(SDLDisplay *display);

/**
 * @brief affiche deux vues dans une seule fenêtre.
 *
 * Le premier monde est affiché à gauche.
 * Le deuxième monde est affiché à droite.
 *
 * pour l'instant, Render_world appelle cette fonction avec le même monde
 * deux fois? plus tard, appeler directement :
 * Render_two_worlds(display, world_bot, world_user);
 *
 * @param display structure SDL contenant la fenêtre et le renderer
 * @param left_world monde affiché à gauche
 * @param right_world monde affiché à droite
 */
void Render_two_worlds(SDLDisplay *display, World *left_world,
                       World *right_world, AudioManager *audio);

#endif
