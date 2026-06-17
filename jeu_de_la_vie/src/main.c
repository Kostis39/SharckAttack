#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"
#include "cell.h"
#include "mj.h"
#include "agent.h"
#include "terminalDisplay.h"

#define WORLD_SIZE 10

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    srand(time(NULL));

    World *w = InitWorld(WORLD_SIZE);
    RandomizeWorld(w);

    printf("World: size=%d, zoom=%.2f, OffsetX=%d, OffsetY=%d\n",
           w->size, w->zoomDisplay, w->OffsetX, w->OffsetY);

    WorldToDisplay *affichage = WorldToDisplayFromWorld(w);

    return Display(affichage);
    // bool program_on = true;

    // while (program_on)
    // {
    // }

    DeleteWorld(w);

    return 0;
}