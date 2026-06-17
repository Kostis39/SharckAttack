#include "world.h"

#define COEFF_ZOOM 0.1f

// Fonction World

World *InitWorld(int size)
{
    World *world = calloc(size, sizeof(World));
    world->tab = InitCell2D(size);
    FillCell2DToFalse(world->tab, size);
    world->OffsetX = 0;
    world->OffsetY = 0;
    world->size = size;
    world->zoomDisplay = 1;
    return world;
}

void RandomizeWorld(World *w)
{
    for (int y = 0; y < w->size; y++)
    {
        for (int x = 0; x < w->size; x++)
        {
            w->tab[y][x].state = (rand() % 2 == 0);
        }
    }
}

Cell *GetCellWorld(World *w, int x, int y)
{
    int nx = ((x % w->size) + w->size) % w->size;
    int ny = ((y % w->size) + w->size) % w->size;
    return &w->tab[nx][ny];
}

int GetSizeWorld(World *w)
{
    return w->size;
}

float GetZoomWorld(World *w)
{
    return w->zoomDisplay;
}

int GetOffsetXWorld(World *w)
{
    return w->OffsetX;
}

int GetOffsetYWorld(World *w)
{
    return w->OffsetY;
}

Cell **GetPerseption(World *w, int x, int y)
{
    Cell **perception = calloc(3, sizeof(Cell *));
    if (!perception)
        return NULL;

    for (int i = 0; i < 3; i++)
    {
        perception[i] = calloc(3, sizeof(Cell));
        if (!perception[i])
        {
            for (int j = 0; j < i; j++)
                free(perception[j]);
            free(perception);
            return NULL;
        }

        for (int j = 0; j < 3; j++)
        {
            perception[i][j] = *GetCellWorld(w, x - 1 + j, y - 1 + i);
        }
    }
    return perception;
}

void DeletePerception(Cell **perception)
{
    if (!perception)
        return;
    for (int i = 0; i < 3; i++)
    {
        free(perception[i]);
    }
    free(perception);
}

void IncrementZoom(World *w)
{
    w->zoomDisplay += COEFF_ZOOM;
    if (w->zoomDisplay > w->size)
        w->zoomDisplay = w->size;
}

void DecrementZoom(World *w)
{
    w->zoomDisplay -= COEFF_ZOOM;
    if (w->zoomDisplay < 1)
        w->zoomDisplay = 1;
}

void IncrementOffsetX(World *w)
{
    w->OffsetX++;
}

void DecrementOffsetX(World *w)
{
    w->OffsetX--;
}

void IncrementOffsetY(World *w)
{
    w->OffsetY++;
}

void DecrementOffsetY(World *w)
{
    w->OffsetY--;
}

void DeleteWorld(World *w)
{
    if (!w)
        return;
    for (int i = 0; i < w->size; i++)
    {
        free(w->tab[i]);
    }
    free(w->tab);
}

void PrintInfoWorld(World *w)
{
    printf("World : size=%d, zoom=%.2f, OffsetX=%d, OffsetY=%d\n", w->size, w->zoomDisplay, w->OffsetX, w->OffsetY);
}

// Fonction World Display

Cell **GetTabOfWorldToDisplay(WorldToDisplay *wtd)
{
    return wtd->tab;
}

int GetSizeOfWorldToDisplay(WorldToDisplay *wtd)
{
    return wtd->size;
}

WorldToDisplay *WorldToDisplayFromWorld(World *w)
{
    WorldToDisplay *result = calloc(1, sizeof(WorldToDisplay));
    if (!result)
        return NULL;

    result->size = w->size / w->zoomDisplay;

    result->tab = calloc(result->size, sizeof(Cell *));
    if (!result->tab)
        return NULL;

    for (int i = 0; i < result->size; i++)
    {
        result->tab[i] = calloc(result->size, sizeof(Cell));
        if (!result->tab[i])
        {
            for (int j = 0; j < i; j++)
                free(result->tab[j]);
            free(result->tab);
            return NULL;
        }

        for (int j = 0; j < result->size; j++)
        {
            result->tab[i][j] = *GetCellWorld(w, w->OffsetX + j, w->OffsetY + i);
        }
    }
    return result;
}

void FreeWorldToDisplay(WorldToDisplay *wtd)
{
    if (!wtd)
        return;
    for (int i = 0; i < wtd->size; i++)
    {
        free(wtd->tab[i]);
    }
    free(wtd->tab);
}