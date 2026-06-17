#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include "sprites_config.h"

void draw_background(SDL_Renderer *renderer,
                     SDL_Texture *background,
                     int scroll_x);

void draw_ground(SDL_Renderer *renderer);

#endif