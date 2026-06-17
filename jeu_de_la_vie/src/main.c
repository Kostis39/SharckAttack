#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "cell.h"
#include "mj.h"
#include "agent.h"
#include "terminalDisplay.h"

#define WORLD_SIZE 50

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    World *w = InitWorld(WORLD_SIZE);
    RandomizeWorld(w);

    printf("World: size=%d, zoom=%.2f, OffsetX=%d, OffsetY=%d\n",
           w->size, w->zoomDisplay, w->OffsetX, w->OffsetY);

    WorldToDisplay *affichage = WorldToDisplayFromWorld(w);

    int ok = Display(affichage);
    // bool program_on = true;

    // while (program_on)
    // {
    // }

    DeleteWorld(w);

    return 0;
}