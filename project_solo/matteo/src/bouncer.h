#ifndef BOUNCER_H
#define BOUNCER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>

typedef struct RGBColor {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} RGBColor;

RGBColor hsv2rgb(float H, float S, float V);
void spawnBounce(int size, float speed);

#endif
