#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "mj.h"
#include "agent.h"
#include "SDLDisplay.h"

#define WORLD_SIZE 50

void SDLUserEvent(SDL_Event event, World *world)
{
    switch (event.type)
    {
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

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    World *world = InitWorld(WORLD_SIZE);

    bool program_on = true;

    while (program_on)
    {
    }

    DeleteWorld(world);

    return 0;
}