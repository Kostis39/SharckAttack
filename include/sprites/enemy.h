#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

void draw_enemy(SDL_Renderer *renderer,
                SDL_Texture *enemy_texture,
                SDL_Rect enemy,
                int enemy_life);

void draw_life_bar(SDL_Renderer *renderer,
                   SDL_Rect enemy,
                   int life_value);

#endif