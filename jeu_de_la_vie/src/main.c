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
int SDLUserEvent(SDL_Event event, World *world, int *isPaused) {
    /**
     * @brief Gère les actions possible à partir de SDL et execute les actions
     * adéquat: Si zqsd : déplace la vision du monde respectivement en haut
     * droite bas gauche Si a et e : zoom la vision du monde Si espace : passe à
     * l'itération suivante
     * @param event L'événement à traiter.
     * @param world Le monde à modifier.
     * @return 0 si la boucle est interrompue, avec SPC ou SDL_QUIT
     * @return 1 sinon
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
int TerminalUserEvent(World *world) {
    /**
     * @brief Gère les actions possible à partir de SDL et execute les actions
     * adéquat: Si zqsd : déplace la vision du monde respectivement en haut
     * droite bas gauche Si a et e : zoom la vision du monde Si espace : passe à
     * l'itération suivante
     * @param event L'événement à traiter.
     * @param world Le monde à modifier.
     * @return 0 si la boucle est interrompue, avec SPC ou SDL_QUIT
     * @return 1 sinon
     */
    char input;
    scanf("%c%*c", &input);
    switch (input) {
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

/**
 * \brief Point d'entrée du programme.
 *
 * Initialise le monde, le remplit aléatoirement et lance l'affichage terminal.
 *
 * \param argc Nombre d'arguments.
 * \param argv Tableau d'arguments.
 * \return EXIT_SUCCESS en cas de succès.
 */
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    srand(time(NULL));

    World *w = InitWorld(WORLD_SIZE);
    RandomizeWorld(w);

    PrintInfoWorld(w);

    WorldToDisplay *affichage = WorldToDisplayFromWorld(w);

    return Display(affichage);
    // bool program_on = true;

    // while (program_on)
    // {
    // }

    DeleteWorld(w);

    return 0;
}
