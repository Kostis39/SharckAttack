#include "cell.h"

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

Cell **InitCell2D(int size)
{
    Cell **tab = calloc(size, sizeof(Cell *));
    for (int y = 0; y < size; ++y)
    {
        tab[y] = calloc(size, sizeof(Cell));
    }
    return tab;
}

void FillCell2DToFalse(Cell **tab, int size)
{
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            SetStateCell(&tab[y][x], false);
        }
    }
}
