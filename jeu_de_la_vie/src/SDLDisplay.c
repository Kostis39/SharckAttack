#include "../include/SDLDisplay.h"

#include <stdio.h>

#define BACKGROUND_R 20
#define BACKGROUND_G 20
#define BACKGROUND_B 20

#define CELL_R 230
#define CELL_G 230
#define CELL_B 230

// Permet d'avoir un modulo toujours positif
int ModuloPositive(int value, int modulo)
{
    int result = value % modulo;

    if (result < 0)
    {
        result += modulo;
    }

    return result;
}

// Initialise la fenêtre SDL
bool InitSDLDisplay(SDLDisplay *display, int windowSize)
{
    if (display == NULL)
    {
        return false;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return false;
    }

    display->windowSize = windowSize;

    display->window = SDL_CreateWindow(
        "Jeu de la Vie",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowSize,
        windowSize,
        SDL_WINDOW_SHOWN
    );

    if (display->window == NULL)
    {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    display->renderer = SDL_CreateRenderer(
        display->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (display->renderer == NULL)
    {
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(display->window);
        SDL_Quit();
        return false;
    }

    return true;
}

// Détruit la fenêtre SDL
void DestroySDLDisplay(SDLDisplay *display)
{
    if (display == NULL)
    {
        return;
    }

    if (display->renderer != NULL)
    {
        SDL_DestroyRenderer(display->renderer);
        display->renderer = NULL;
    }

    if (display->window != NULL)
    {
        SDL_DestroyWindow(display->window);
        display->window = NULL;
    }

    SDL_Quit();
}

// Efface la fenêtre
void ClearSDLDisplay(SDLDisplay *display)
{
    if (display == NULL || display->renderer == NULL)
    {
        return;
    }

    SDL_SetRenderDrawColor(
        display->renderer,
        BACKGROUND_R,
        BACKGROUND_G,
        BACKGROUND_B,
        255
    );

    SDL_RenderClear(display->renderer);
}

// Donne la taille d'une cellule affichée
int GetCellSizeSDLDisplay(SDLDisplay *display, WorldToDisplay *worldToDisplay)
{
    if (display == NULL || worldToDisplay == NULL)
    {
        return 1;
    }

    int displaySize = GetSizeOfWorldToDisplay(worldToDisplay);

    if (displaySize <= 0)
    {
        return 1;
    }

    int cellSize = display->windowSize / displaySize;

    if (cellSize < 1)
    {
        cellSize = 1;
    }

    return cellSize;
}

