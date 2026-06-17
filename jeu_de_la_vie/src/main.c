/**
 * \file main.c
 * \brief Point d'entrée du programme — boucle principale du Jeu de la Vie.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDLDisplay.h"
#include "SDL_events.h"
#include "agent.h"
#include "cell.h"
#include "mj.h"
#include "terminalDisplay.h"
#include "world.h"

/** \brief Taille par défaut de la grille du monde. */
#define WORLD_SIZE 10

int SDLUserEvent(SDL_Event event, World *world, int *isPaused) {
    /**
     * \brief Gère les événements SDL (clavier, quit).
     *
     * Touches supportées :
     * - Z/Q/S/D : déplacer la vue
     * - A/E : zoomer / dézoomer
     * - Espace : pause
     * - Échap / fermeture : quitter
     *
     * \param event Événement SDL à traiter.
     * \param world Pointeur vers le monde.
     * \param isPaused Pointeur vers l'état de pause.
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
            *isPaused = !*isPaused;
            break;
        case SDLK_z:
            DecrementOffsetY(world);
            break;
        case SDLK_s:
            IncrementOffsetY(world);
            break;
        case SDLK_q:
            DecrementOffsetX(world);
            break;
        case SDLK_d:
            IncrementOffsetX(world);
            break;
        case SDLK_a:
            IncrementZoom(world);
            break;
        case SDLK_e:
            DecrementZoom(world);
            break;
        default:
            break;
        }
    }
    return 1;
}

int TerminalUserEvent(World *world) {
    /**
     * \brief Gère les événements en mode terminal (entrée standard).
     *
     * Touches supportées :
     * - Z/Q/S/D : déplacer la vue
     * - A/E : zoomer / dézoomer
     * - K : quitter
     *
     * \param world Pointeur vers le monde.
     * \return 0 pour quitter, 1 pour continuer.
     */
    char input;
    scanf("%c", &input);
    switch (input) {
    case '\n':
        *next_iteration = true;
        break;
    case 'z':
        DecrementOffsetY(world);
        break;
    case 's':
        IncrementOffsetY(world);
        break;
    case 'q':
        DecrementOffsetX(world);
        break;
    case 'd':
        IncrementOffsetX(world);
        break;
    case 'a':
        IncrementZoom(world);
        break;
    case 'e':
        DecrementZoom(world);
        break;
    case 'k': // kill
        return 0;
    default:
        break;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    /**
     * \brief Point d'entrée du programme.
     *
     * Initialise le monde, le remplit aléatoirement et lance l'affichage
     * terminal.
     *
     * \param argc Nombre d'arguments.
     * \param argv Tableau d'arguments.
     * \return 0 si tout s'est bien passé
     */
    (void)argc;
    (void)argv;

    srand(time(NULL));

    World *w = InitWorld(WORLD_SIZE);
    RandomizeWorld(w);

    bool program_on = true;
    bool next_iteration = false;

    WorldToDisplay *display;
    Cell **perception;
    Cell **tmp;

    while (program_on) {
        display = WorldToDisplayFromWorld(w);
        Display(display);

        TerminalUserEvent(w, &next_iteration);

        if (next_iteration) {
            tmp = InitCell2D(w->size);

            for (int y = 0; y < w->size; y++) {
                for (int x = 0; x < w->size; x++) {
                    perception = GetPerseption(
                        w, x, y); // tableau 3x3 autour de la cellule
                    SetValueTabCell(tmp, x, y,
                                    NewState(GetCellWorld(w, x, y),
                                             GetNbNeighbors(perception)));
                }
            }

            SwitchTabCellWorld(w, tmp);

            next_iteration = false;
        }
    }

    // Libération du tableau temporaire
    for (int i = 0; i < w->size; i++)
        free(tmp[i]);
    free(tmp);

    DeletePerception(perception);

    FreeWorldToDisplay(display);

    DeleteWorld(w);

    return 0;
}

/*
(void)argc;
(void)argv;

srand(time(NULL));

// Création du monde
World *world = InitWorld(WORLD_SIZE);

if (world == NULL)
{
printf("Erreur : impossible de créer le monde\n");
return 1;
}

// Remplissage aléatoire du monde
RandomizeWorld(world);

// Création de l'affichage SDL
SDLDisplay display;

if (!InitSDLDisplay(&display, WINDOW_SIZE))
{
printf("Erreur : impossible d'initialiser SDLDisplay\n");
DeleteWorld(world);
free(world);
return 1;
}

bool programOn = true;

while (programOn)
{
SDL_Event event;

// On gère seulement la fermeture de la fenêtre
while (SDL_PollEvent(&event))
{
if (event.type == SDL_QUIT)
{
programOn = false;
}
}

// On prépare uniquement la partie du monde à afficher
WorldToDisplay *worldToDisplay = WorldToDisplayFromWorld(world);

if (worldToDisplay != NULL)
{
// Affichage SDL
RenderSDLDisn cas de succès.play(&display, worldToDisplay);

// Libération du WorldToDisplay
FreeWorldToDisplay(worldToDisplay);
free(worldToDisplay);
}

SDL_Delay(16);
}

// Nettoyage final
DestroySDLDisplay(&display);

DeleteWorld(world);
free(world);

return 0;
*/