/**
 * \file SDLDisplay.c
 * \brief Implémentation de l'affichage SDL2 pour le Jeu de la Vie.
 */

#include "SDLDisplay.h"
#include "world.h"

#include <stdio.h>

/*
    Couleurs utilisées pour l'affichage.
    Une cellule vivante est noire.
    Une cellule morte est blanche.
*/

#define ALIVE_CELL_R 0
#define ALIVE_CELL_G 0
#define ALIVE_CELL_B 0

#define DEAD_CELL_R 255
#define DEAD_CELL_G 255
#define DEAD_CELL_B 255

#define GRID_R 0
#define GRID_G 100
#define GRID_B 255

#define BACKGROUND_R 20
#define BACKGROUND_G 20
#define BACKGROUND_B 20

// Initialise la fenêtre SDL
bool InitSDLDisplay(SDLDisplay *display, int windowSize) {
    if (display == NULL) {
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return false;
    }

    display->windowSize = windowSize;

    display->window = SDL_CreateWindow("Jeu de la Vie", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, windowSize,
                                       windowSize, SDL_WINDOW_SHOWN);

    if (display->window == NULL) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    display->renderer = SDL_CreateRenderer(display->window, -1,
                                           SDL_RENDERER_ACCELERATED |
                                               SDL_RENDERER_PRESENTVSYNC);

    if (display->renderer == NULL) {
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(display->window);
        SDL_Quit();
        return false;
    }

    return true;
}

// Détruit proprement la fenêtre et le renderer
void DestroySDLDisplay(SDLDisplay *display) {
    if (display == NULL) {
        return;
    }

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

// Efface la fenêtre avant de redessiner
void ClearSDLDisplay(SDLDisplay *display) {
    if (display == NULL || display->renderer == NULL) {
        return;
    }

    SDL_SetRenderDrawColor(display->renderer, BACKGROUND_R, BACKGROUND_G,
                           BACKGROUND_B, 255);

    SDL_RenderClear(display->renderer);
}

// Calcule la taille d'une cellule en pixels
int GetCellSizeSDLDisplay(SDLDisplay *display, WorldToDisplay *worldToDisplay) {
    if (display == NULL || worldToDisplay == NULL) {
        return 1;
    }

    int displaySize = GetSizeOfWorldToDisplay(worldToDisplay);

    if (displaySize <= 0) {
        return 1;
    }

    int cellSize = display->windowSize / displaySize;

    if (cellSize < 1) {
        cellSize = 1;
    }

    return cellSize;
}

// Affiche le tableau reçu sous forme de grille SDL
void RenderSDLDisplay(SDLDisplay *display, WorldToDisplay *worldToDisplay) {
    if (display == NULL || worldToDisplay == NULL) {
        return;
    }

    ClearSDLDisplay(display);

    Cell **tab = GetTabOfWorldToDisplay(worldToDisplay);
    int displaySize = GetSizeOfWorldToDisplay(worldToDisplay);
    int cellSize = GetCellSizeSDLDisplay(display, worldToDisplay);

    if (tab == NULL || displaySize <= 0) {
        SDL_RenderPresent(display->renderer);
        return;
    }

    for (int y = 0; y < displaySize; y++) {
        for (int x = 0; x < displaySize; x++) {
            SDL_Rect cellRect;

            cellRect.x = x * cellSize;
            cellRect.y = y * cellSize;
            cellRect.w = cellSize;
            cellRect.h = cellSize;

            if (IsAlive(&(tab[x][y]))) {
                // Cellule vivante : noir
                SDL_SetRenderDrawColor(display->renderer, ALIVE_CELL_R,
                                       ALIVE_CELL_G, ALIVE_CELL_B, 255);
            } else {
                // Cellule morte : blanc
                SDL_SetRenderDrawColor(display->renderer, DEAD_CELL_R,
                                       DEAD_CELL_G, DEAD_CELL_B, 255);
            }

            SDL_RenderFillRect(display->renderer, &cellRect);

            // Dessine le contour de la cellule pour voir la grille
            if (cellSize >= 3) {
                SDL_SetRenderDrawColor(display->renderer, GRID_R, GRID_G,
                                       GRID_B, 255);

                SDL_RenderDrawRect(display->renderer, &cellRect);
            }
        }
    }

    SDL_RenderPresent(display->renderer);
}