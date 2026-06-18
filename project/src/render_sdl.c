#include "render_sdl.h"
#include <stdio.h>
#include <stdlib.h>

bool Init_sdl_display(SDLDisplay *display, char *title, int width, int height) {
    if (display == NULL) {
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur : %s\n", SDL_GetError());
        return false;
    }

    display->window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         width, height, SDL_WINDOW_SHOWN);

    if (display->window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    display->renderer =
        SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);

    if (display->renderer == NULL) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(display->window);
        SDL_Quit();
        return false;
    }

    return true;
}

void Destroy_sdl_display(SDLDisplay *display) {
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

void Clear_sdl_display(SDLDisplay *display) {
    if (display == NULL || display->renderer == NULL)
        return;

    SDL_SetRenderDrawColor(display->renderer, 10, 20, 35, 255);
    SDL_RenderClear(display->renderer);
}

void Draw_fish(SDLDisplay *display, Fish *fish) {
    if (display == NULL || display->renderer == NULL || fish == NULL)
        return;

    if (fish->is_alive == false)
        return;

    int x = fish->position.x;
    int y = fish->position.y;

    int x1 = x + FISH_SIZE;
    int y1 = y;

    int x2 = x - FISH_SIZE;
    int y2 = y - FISH_SIZE;

    int x3 = x - FISH_SIZE;
    int y3 = y + FISH_SIZE;

    SDL_SetRenderDrawColor(display->renderer, 0, 0, 255, 255);

    SDL_RenderDrawLine(display->renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(display->renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(display->renderer, x3, y3, x1, y1);
}

void Draw_shark(SDLDisplay *display, Shark *shark) {
    if (display == NULL || display->renderer == NULL || shark == NULL)
        return;

    int x = shark->pos.x;
    int y = shark->pos.y;

    int x1 = x + SHARK_SIZE;
    int y1 = y;

    int x2 = x - SHARK_SIZE;
    int y2 = y - SHARK_SIZE;

    int x3 = x - SHARK_SIZE;
    int y3 = y + SHARK_SIZE;

    SDL_SetRenderDrawColor(display->renderer, 255, 0, 0, 255);

    SDL_RenderDrawLine(display->renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(display->renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(display->renderer, x3, y3, x1, y1);
}

void Draw_world(SDLDisplay *display, Fish *fishes, int nb_fish, Shark *shark) {
    if (display == NULL || display->renderer == NULL)
        return;

    Clear_sdl_display(display);  // efface l'ancien ecran

    for (int i = 0; i < nb_fish; i++) {
        Draw_fish(display, &fishes[i]); // dessine les poissons
    }

    Draw_shark(display, shark); // dessine le requin

    SDL_RenderPresent(display->renderer); // affiche le resultat
}

void Render_world(SDLDisplay *display, World *world) {
    if (display == NULL || world == NULL)
        return;

    Draw_world(display, world->fishes, world->nb_fish, world->shark);
}