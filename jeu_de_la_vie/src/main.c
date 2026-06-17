#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "mj.h"
#include "agent.h"

#define WORLD_SIZE 50

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