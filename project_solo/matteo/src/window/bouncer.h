#ifndef BOUNCER_H
#define BOUNCER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>

typedef struct RGBColor {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} RGBColor;

typedef struct BouncingWindow {
    SDL_Window *bouncer;
    float x;
    float y;
    float velocity[2];

    int maxX;
    int maxY;

    // Speed Variables
    float speed;
    float minSpeed;
    float maxSpeed;
    float minSize;
    float maxSize;
    float speedFactor;
    int size;
} BouncingWindow_t;

typedef struct RendererBounce {
    Uint32 prevTime;
    RGBColor color;
    float hue;
    SDL_Renderer *ren;
} RendererBounce_t;

RGBColor hsv2rgb(float H, float S, float V);
void spawnBounce_old(int size, float speed);
BouncingWindow_t *spawnBounce(int size, float speed, int maxX, int MaxY);
BouncingWindow_t *UpdateBounce(BouncingWindow_t *bounce);
RendererBounce_t *RenderBounce(RendererBounce_t *ren);

#endif
