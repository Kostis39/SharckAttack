#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"
#include "cell.h"
#include "mj.h"
#include "agent.h"
#include "SDLDisplay.h"
#include "terminalDisplay.h"

#define WORLD_SIZE 10

void SDLUserEvent(SDL_Event event, World *world, bool *next_iteration)
{
    /**
     * @brief Gère les actions possible à partir de SDL et execute les actions adéquat:
     * Si zqsd : déplace la vision du monde respectivement en haut droite bas gauche
     * Si a et e : zoom la vision du monde
     * Si espace : passe à l'itération suivante
     * @param event L'événement à traiter.
     * @param world Le monde à modifier.
     * @param next_iteration Un boolean notifiant si on doit passer à la nouvelle itération.
     */
    switch (event.type)
    {
    case SDLK_SPACE:
        *next_iteration = true;
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

void TerminalUserEvent(char *entry, World *world, bool *next_iteration)
{
    /**
     * @brief Gère les actions possible à partir du terminal et execute les actions adéquat:
     * Si zqsd : déplace la vision du monde respectivement en haut droite bas gauche
     * Si a et e : zoom la vision du monde
     * Si espace : passe à l'itération suivante
     * @param entry L'événement à traiter.
     * @param world Le monde à modifier.
     * @param next_iteration Un boolean notifiant si on doit passer à la nouvelle itération.
     */
    if (entry == NULL)
        return;

    switch (entry[0])
    {
    case '\0':
        *next_iteration = true;
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
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
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