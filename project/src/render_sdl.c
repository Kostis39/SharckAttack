#include <stdio.h>
#include <stdlib.h>
#include "render_sdl.h"

bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height) {
    if(display == NULL) {
        return false;
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur : %s\n", SDL_GetError());
        return false;
    }

    display->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (display->window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    display->renderer = SDL_CreateRenderer(
        display->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (display->renderer == NULL) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(display->window);
        SDL_Quit();
        return false;
    }

    return true;
}

void destroy_sdl_display(SDLDisplay *display)
{
    if (display == NULL)
        return;

    if (display->renderer != NULL) {
        SDL_DestroyRenderer(display->renderer);
        display->renderer = NULL;
    }

    if (display->window != NULL) {
        SDL_DestroyWindow(display->window);
        display->window = NULL;
    }

    SDL_Quit();
}

void clear_sdl_display(SDLDisplay *display)
{
    if (display == NULL || display->renderer == NULL)
        return;

    SDL_SetRenderDrawColor(display->renderer, 10, 20, 35, 255);
    SDL_RenderClear(display->renderer);
}
