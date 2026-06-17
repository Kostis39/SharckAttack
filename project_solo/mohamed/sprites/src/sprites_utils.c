#include "sprites_utils.h"
#include <stdio.h>

void clean(SDL_Window *window,
           SDL_Renderer *renderer,
           SDL_Texture *background,
           SDL_Texture *player,
           SDL_Texture *enemy,
           TTF_Font *font) {
    if (font != NULL) {
        TTF_CloseFont(font);
    }

    if (enemy != NULL) {
        SDL_DestroyTexture(enemy);
    }

    if (player != NULL) {
        SDL_DestroyTexture(player);
    }

    if (background != NULL) {
        SDL_DestroyTexture(background);
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *load_texture(SDL_Renderer *renderer,
                          const char *path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);

    if (texture == NULL) {
        printf("Image non chargee : %s\n", path);
        printf("Erreur SDL_image : %s\n", IMG_GetError());
        return NULL;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

void draw_text(SDL_Renderer *renderer,
               TTF_Font *font,
               const char *text,
               int x,
               int y) {
    if (font == NULL) {
        return;
    }

    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);

    if (surface == NULL) {
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == NULL) {
        return;
    }

    SDL_Rect dst = {x, y, 0, 0};

    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
}