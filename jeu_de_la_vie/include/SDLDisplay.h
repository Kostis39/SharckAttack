#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "world.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int windowSize;

} SDLDisplay;

// Initialise la fenêtre SDL
bool InitSDLDisplay(SDLDisplay *display, int windowSize);

// Détruit la fenêtre SDL
void DestroySDLDisplay(SDLDisplay *display);

// Efface la fenêtre
void ClearSDLDisplay(SDLDisplay *display);

// Affiche le tableau donné
void RenderSDLDisplay(SDLDisplay *display, WorldToDisplay *worldToDisplay);

// Donne la taille d'une cellule affichée
int GetCellSizeSDLDisplay(SDLDisplay *display, WorldToDisplay *worldToDisplay);

#endif