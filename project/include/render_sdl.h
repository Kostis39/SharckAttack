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
} SDLDisplay;

bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height);
void Destroy_sdl_display(SDLDisplay *display);

void CLear_sdl_display(SDLDisplay *display);
void Render_world(SDLDisplay *display, World *world);

void Draw_fish(SDL_Renderer *renderer, Fish *fish);
void Draw_shark(SDL_Renderer *renderer, Shark *shark);

#endif