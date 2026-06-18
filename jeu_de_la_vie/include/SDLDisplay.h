#ifndef SDL_DISPLAY_H
#define SDL_DISPLAY_H

/**
 * \file SDLDisplay.h
 * \brief Définition de la structure SDLDisplay et des fonctions d'affichage SDL2.
 *
 * Ce fichier contient la structure SDLDisplay utilisée pour gérer la fenêtre
 * SDL2, le renderer et la taille de l'affichage. Il déclare aussi les fonctions
 * permettant d'initialiser l'affichage, de dessiner le monde du Jeu de la Vie
 * et de libérer les ressources SDL.
 */

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "world.h"

/**
 * \struct SDLDisplay
 * \brief Représente les éléments nécessaires pour afficher le Jeu de la Vie avec SDL2.
 */
typedef struct
{
    SDL_Window *window;       /**< Fenêtre SDL utilisée pour l'affichage. */
    SDL_Renderer *renderer;   /**< Renderer SDL utilisé pour dessiner. */

    int windowSize;           /**< Taille de la fenêtre en pixels. */

} SDLDisplay;

// Initialise la fenêtre SDL
bool InitSDLDisplay(SDLDisplay *display, int windowSize);

// Détruit la fenêtre SDL
void DestroySDLDisplay(SDLDisplay *display);

// Efface la fenêtre
void ClearSDLDisplay(SDLDisplay *display);

// Affiche le tableau donné
void RenderSDLDisplay(SDLDisplay *display, WorldToDisplay *worldToDisplay);

// Donne la taille d'une cellule affichée
int GetCellSizeSDLDisplay(SDLDisplay *display, WorldToDisplay *worldToDisplay);

#endif