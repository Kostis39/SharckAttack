#ifndef MJ_H
#define MJ_H

#include "cell.h"

typedef struct
{
    Cell **tab;
    int size;
    int zommDisplay;        // Taille du tableau à afficher
    int xDisplay, yDisplay; // Coordonnées du coin supérieur gauche pour le tableau à afficher
} World;

#endif
