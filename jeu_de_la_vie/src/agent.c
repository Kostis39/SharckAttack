#include "agent.h"

int GetNbNeighbors(Cell **c)
{
    if (c == NULL)
    {
        printf("Error NULL in GetNbNeighbors");
        return 0;
    }

    int nbNeightbors = 0;
    int around = 3;

    for (int y = 0; y < around; ++y)
    {
        for (int x = 0; x < around; ++x)
        {
            ++nbNeightbors;
        }
    }
    return nbNeightbors;
}

bool NewState(Cell *c, int nbNeightbors)
{
    if (GetStateCell(c))
    {
        return nbNeightbors == 2 || nbNeightbors == 3;
    }
    else
    {
        return nbNeightbors == 3;
    }
}