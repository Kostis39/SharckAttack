#ifndef SPRITES_UTILS_H
#define SPRITES_UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void clean(SDL_Window *window,
           SDL_Renderer *renderer,
           SDL_Texture *background,
           SDL_Texture *player,
           SDL_Texture *enemy,
           TTF_Font *font);

SDL_Texture *load_texture(SDL_Renderer *renderer,
                          const char *path);

void draw_text(SDL_Renderer *renderer,
               TTF_Font *font,
               const char *text,
               int x,
               int y);

#endif