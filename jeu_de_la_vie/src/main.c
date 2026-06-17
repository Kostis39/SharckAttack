/**
 * \file main.c
 * \brief Point d'entrée du programme — boucle principale du Jeu de la Vie.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDLDisplay.h"
#include "agent.h"
#include "cell.h"
#include "mj.h"
#include "terminalDisplay.h"
#include "world.h"

/** \brief Taille par défaut de la grille du monde. */
#define WORLD_SIZE 10

/** \brief Taille de la fenêtre SDL*/
#define WINDOW_SIZE 800

int SDLUserEvent(SDL_Event event, World *world, bool *next_iteration) {
    /**
     * \brief Gère les événements SDL (clavier, quit).
     *
     * Touches supportées :
     * - Z/Q/S/D : déplacer la vue
     * - Scroll molette : zoomer / dézoomer
     * - Espace : nouvelle itération
     * - Échap : quitter
     *
     * \param event Événement SDL à traiter.
     * \param world Pointeur vers le monde.
     * \param next_iteration Pointeur vers l'info qui indique si on change
     * d'itération.
     * \return 0 pour quitter, 1 pour continuer.
     */
    switch (event.type) {
    case SDL_QUIT:
        return 0;

    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            return 0;
        case SDLK_SPACE:
            *next_iteration = true;
            break;
        case SDLK_z:
            DecrementOffsetX(world);
            break;
        case SDLK_s:
            IncrementOffsetX(world);
            break;
        case SDLK_q:
            DecrementOffsetY(world);
            break;
        case SDLK_d:
            IncrementOffsetY(world);
            break;
        default:
            break;
        }
        break;

    case SDL_MOUSEWHEEL:
        if (event.wheel.y > 0)
            IncrementZoom(world);
        else if (event.wheel.y < 0)
            DecrementZoom(world);
        break;

    default:
        break;
    }

    return 1;
}

int TerminalUserEvent(World *world, bool *next_iteration) {
    /**
     * \brief Gère les événements en mode terminal (entrée standard).
     *
     * Touches supportées :
     * - Z/Q/S/D : déplacer la vue
     * - A/E : zoomer / dézoomer
     * - K : quitter
     * - N : nouvelle itération
     *
     * \param world Pointeur vers le monde.
     * \param next_iteration Pointeur vers l'info qui indique si on change
     * d'itération.
     * \return 0 pour quitter, 1 pour continuer.
     */
    char input;
    scanf(" %c", &input);
    switch (input) {
    case 'n':
        *next_iteration = true;
        break;
    case 'z':
        DecrementOffsetX(world);
        break;
    case 's':
        IncrementOffsetX(world);
        break;
    case 'q':
        DecrementOffsetY(world);
        break;
    case 'd':
        IncrementOffsetY(world);
        break;
    case 'a':
        DecrementZoom(world);
        break;
    case 'e':
        IncrementZoom(world);
        break;
    case 'k': // kill
        return 0;
    default:
        break;
    }
    return 1;
}

void runTerminal(World *w) {
    bool program_on = true;
    bool next_iteration = false;
    WorldToDisplay *display;
    Cell **perception;
    Cell **tmp = InitCell2D(w->size);

    RandomizeWorld(w);

    while (program_on) {
        display = WorldToDisplayFromWorld(w);
        Display(display);
        FreeWorldToDisplay(display);

        if (!TerminalUserEvent(w, &next_iteration))
            program_on = false;

        if (next_iteration) {
            FillCell2DToFalse(tmp, w->size);

            for (int y = 0; y < w->size; y++) {
                for (int x = 0; x < w->size; x++) {
                    perception = GetPerseption(w, x, y);

                    SetValueTabCell(tmp, x, y,
                                    NewState(GetCellWorld(w, x, y),
                                             GetNbNeighbors(perception)));

                    DeletePerception(perception);
                }
            }

            SwitchTabCellWorld(w, tmp);
            next_iteration = false;
        }
    }

    DeleteCell2D(tmp, w->size);
}

void runSDL(World *w) {
    SDLDisplay display;
    if (!InitSDLDisplay(&display, WINDOW_SIZE)) {
        fprintf(stderr, "Erreur : impossible d'initialiser SDL\n");
        return;
    }

    bool program_on = true;
    bool next_iteration = false;
    WorldToDisplay *worldToDisplay;
    Cell **perception;
    Cell **tmp = InitCell2D(w->size);

    RandomizeWorld(w);

    while (program_on) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (!SDLUserEvent(event, w, &next_iteration))
                program_on = false;
        }

        worldToDisplay = WorldToDisplayFromWorld(w);
        RenderSDLDisplay(&display, worldToDisplay);
        FreeWorldToDisplay(worldToDisplay);

        if (next_iteration) {
            FillCell2DToFalse(tmp, w->size);

            for (int y = 0; y < w->size; y++) {
                for (int x = 0; x < w->size; x++) {
                    perception = GetPerseption(w, x, y);

                    SetValueTabCell(tmp, x, y,
                                    NewState(GetCellWorld(w, x, y),
                                             GetNbNeighbors(perception)));

                    DeletePerception(perception);
                }
            }

            SwitchTabCellWorld(w, tmp);
            next_iteration = false;
        }

        SDL_Delay(10);
    }

    DeleteCell2D(tmp, w->size);
    DestroySDLDisplay(&display);
}

int main(int argc, char *argv[]) {
    (void)argc;

    srand(time(NULL));

    World *w = InitWorld(WORLD_SIZE);

    if (argc > 1 && strcmp(argv[1], "sdl") == 0) {
        runSDL(w);
    } else {
        runTerminal(w);
    }

    DeleteWorld(w);

    return 0;
}