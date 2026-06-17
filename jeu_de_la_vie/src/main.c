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

void TerminalUserEvent(char entry, World *world, bool *next_iteration)
{
    switch (entry)
    {
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

    bool program_on = true;
    bool next_iteration = false;
    char entry;

    WorldToDisplay *display;
    Cell **perception;
    Cell **tmp;

    while (program_on)
    {
        display = WorldToDisplayFromWorld(w);
        Display(display);

        scanf("%c", &entry);

        TerminalUserEvent(entry, w, &next_iteration);

        if (next_iteration)
        {
            tmp = InitCell2D(w->size);

            for (int y = 0; y < w->size; y++)
            {
                for (int x = 0; x < w->size; x++)
                {
                    perception = GetPerseption(w, x, y); // tableau 3x3 autour de la cellule
                    SetValueTabCell(tmp, x, y, NewState(GetCellWorld(w, x, y), GetNbNeighbors(perception)));
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