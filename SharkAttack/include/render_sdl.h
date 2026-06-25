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

bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height);
void Destroy_sdl_display(SDLDisplay *display);
void Clear_sdl_display(SDLDisplay *display);

void Draw_fish(SDLDisplay *display, Fish *fish);
void Draw_shark(SDLDisplay *display, Shark *shark);

void Draw_world(SDLDisplay *display, World *world);
void Render_two_worlds(SDLDisplay *display, World *left_world,
                       World *right_world, AudioManager *audio);
void Render_world(SDLDisplay *display, World *world);

void Draw_collider(SDLDisplay *display, Collider *collider);
void Draw_colliders(SDLDisplay *display, Collider *colliders, int nb_colliders);

#endif
