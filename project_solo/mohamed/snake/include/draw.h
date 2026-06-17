#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include "snake.h"

void draw_scene(SDL_Renderer *renderer, Snake snakes[], int count, int width, int height, int paused);

#endif
