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

void Draw_fish(SDLDisplay *display, Fish *fish);
void Draw_shark(SDLDisplay *display, Shark *shark);

void Draw_world(SDLDisplay *display, Fish *fiches, int nb_fish, Shark *shark);
void Render_world(SDLDisplay *display, World *world); 

#endif