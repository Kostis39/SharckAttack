#ifndef SDL_DRAW_TOOLS_H
#define SDL_DRAW_TOOLS_H

#include <SDL2/SDL.h>
#include "vector.h"

int to_int(float x);

Vector direction_or_default(Vector v);

void Draw_filled_circle(SDL_Renderer *r, int cx, int cy, int radius);

void Draw_filled_ellipse(SDL_Renderer *r, int cx, int cy, int rx, int ry);

void Draw_filled_triangle(SDL_Renderer *r, int x1, int y1, int x2, int y2,
                          int x3, int y3);

void Draw_wave(SDL_Renderer *r, int width, int base_y, int move, int amplitude,
               float time);

void Draw_bubbles(SDL_Renderer *r, int width, int height);

void Draw_sea_plants(SDL_Renderer *r, int width, int height, float time);

void Draw_mine_shape(SDL_Renderer *r, int cx, int cy, int radius);

void Draw_digit(SDL_Renderer *r, int x, int y, int n, int s);

int Count_digits(int n);

void Draw_circle_outline(SDL_Renderer *r, int cx, int cy, int radius);

void Draw_vector(SDL_Renderer *r, Vector pos, Vector dir, int length);

#endif