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

    bool isDragging;
    int lastMouseX;
    int lastMouseY;

} SDLDisplay;

// Initialisation et destruction de l'affichage SDL

bool InitSDLDisplay(SDLDisplay *display, int windowSize);

void DestroySDLDisplay(SDLDisplay *display);

// Affichage du monde

void RenderSDLDisplay(SDLDisplay *display, World *world);

void ClearSDLDisplay(SDLDisplay *display, World *world);

#endif
