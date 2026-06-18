#ifndef RENDER_SDL_H
#define RENDER_SDL_H

#include <sddbool.h>
#include <SDL2/SDL.h>

#include "world.h"
#include "fish.h"
#include "shark.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} sdl_display;

bool Init_sdl_display(sdl_display *display, char *title, int width, int height);
void Destroy_sdl_display(sdl_display *display);
