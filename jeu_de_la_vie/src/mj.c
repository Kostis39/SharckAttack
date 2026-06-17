#include "mj.h"

void SwitchTabCellWorld(World *worldNow, Cell **worldTmp)
{
    if (worldNow == NULL || worldNow->tab == NULL || worldTmp == NULL)
        return;

    int size = worldNow->size;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            SetStateCell(&worldNow->tab[y][x], GetStateCell(&worldTmp[y][x]));
            SetStateCell(&worldTmp[y][x], false);
        }
    }
}

void SetValueTabCell(Cell **tab, int x, int y, bool val)
{
    SetStateCell(&tab[y][x], val);
}

World *InitWorld(int size)
{
    World *world = calloc(size, sizeof(World));
    world->tab = InitCell2DToFalse(size);
    world->OffsetX = 0;
    world->OffsetY = 0;
    world->size = size;
    world->zoomDisplay = 1;
}
