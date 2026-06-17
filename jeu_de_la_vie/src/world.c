#include "world.h"

#define COEFF_ZOOM 0.1f

// Fonction Cell

bool GetStateCell(Cell *c)
{
    return c->state;
}

void SwitchStateCell(Cell *c)
{
    c->state = !c->state;
}

void SetStateCell(Cell *c, bool val)
{
    c->state = val;
}

Cell **InitCell2DToFalse(int size)
{
    Cell **tab = calloc(size, sizeof(Cell *));
    for (int y = 0; y < size; ++y)
    {
        tab[y] = calloc(size, sizeof(Cell));
    }
    return tab;
}

Cell **InitCell2DToFalse(int size)
{
    Cell **tab = calloc(size, sizeof(Cell *));
    for (int y = 0; y < size; ++y)
    {
        tab[y] = calloc(size, sizeof(Cell));
    }
    return tab;
}

// Fonction World Display

Cell **GetTabOfWorlToDisplay(WorldToDisplay *wtd)
{
    return wtd->tab;
}

int GetSizeOfWorldToDisplay(WorldToDisplay *wtd)
{
    return wtd->size;
}

WorldToDisplay *WorldToDisplayFromWorld(World *w)
{
    WorldToDisplay *result;
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
            result->tab[i][j] = GetCellWorld(w, w->OffsetX + j, w->OffsetY + i);
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

// Fonction World

Cell GetCellWorld(World *w, int x, int y)
{
    return w->tab[x % w->size][y % w->size];
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
            perception[i][j] = GetCellWorld(w, x - 1 + j, y - 1 + i);
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
    w->zoomDisplay + COEFF_ZOOM;
    if (w->zoomDisplay > w->size)
        w->size;
}

void DecrementZoom(World *w)
{
    w->zoomDisplay - COEFF_ZOOM;
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