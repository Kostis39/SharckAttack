#include "world.h"

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

// Fonction World

Cell GetCellWorld(World *w, int x, int y)
{
    return w->tab[x % w->size][y % w->size];
}

int GetSizeWorld(World *w)
{
    return w->size;
}

int GetZoomWorld(World *w)
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
    Cell **perception = malloc(3 * sizeof(Cell *));
    if (!perception)
        return NULL;

    for (int i = 0; i < 3; i++)
    {
        perception[i] = malloc(3 * sizeof(Cell));
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

void IncrementZoom(World *w)
{
    w->zoomDisplay++;
}

void DecrementZoom(World *w)
{
    w->zoomDisplay--;
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