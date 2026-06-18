#ifndef RENDER_SDL_H
#define RENDER_SDL_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "fish.h"
#include "shark.h"
#include "world.h"

/**
 * @struct SDLDisplay
 * @brief Structure représentant l'affichage SDL du jeu.
 */
typedef struct {
    SDL_Window *window;     /**< Pointeur vers la fenêtre SDL */
    SDL_Renderer *renderer; /**< Pointeur vers le rendu SDL */
} SDLDisplay;

bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height);
void Destroy_sdl_display(SDLDisplay *display);

void Clear_sdl_display(SDLDisplay *display);
void Render_world(SDLDisplay *display, World *world);

void Draw_fish(SDL_Renderer *renderer, Fish *fish);
void Draw_shark(SDL_Renderer *renderer, Shark *shark);

#endif