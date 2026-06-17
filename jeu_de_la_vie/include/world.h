#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    bool state; // 0 mort, 1 vie
} Cell;

typedef struct
{
    Cell **tab;
    int size;
    int zommDisplay;      // Taille du tableau à afficher
    int OffsetX, OffsetY; // Coordonnées du coin supérieur gauche pour le tableau à afficher
} World;

// Fonction Cell

bool GetStateCell(Cell *c);
void SwitchStateCell(Cell *c);
void SetStateCell(Cell *c, bool val);

// Fonction World

Cell GetCellWorld(World *w, int x, int y);
int GetSizeWorld(World *w);
int GetZoomWorld(World *w);
int GetOffsetXWorld(World *w);
int GetOffsetYWorld(World *w);

Cell **GetPerseption(World *w, int x, int y);

void IncrementZoom(World *w);
void DecrementZoom(World *w);

void IncrementOffsetX(World *w);
void DecrementOffsetX(World *w);

void IncrementOffsetY(World *w);
void DecrementOffsetY(World *w);

#endif
