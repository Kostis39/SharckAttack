#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include <stdlib.h>
/**
 * \struct Cell
 * \brief Représente une cellule, elle est soit vivante soit morte.
 */
typedef struct
{
  bool state; /**< Staut de la cellule, True: vivant, False: mort. */
} Cell;

/**
 * \struct World
 * \brief Représente le monde du jeu de la vie.
 */
typedef struct
{
  Cell **tab;           /**< Tableau carre 2D de cellules */
  int size;             /**< Taille du tableau, la meme taille en width et height */
  float zoomDisplay;    /**< coefficient du zoom */
  int OffsetX, OffsetY; /**< Coordonnées du coin supérieur gauche, afin de savoir comment l'afficher */
} World;

typedef struct
{
  Cell **tab; /**< Tableau à afficher carre 2D de cellule vivante ou morte */
  int size;   /**< Taille du tableau à afficher, la meme taille en width et height */
} WorldToDisplay;

// Fonction Cell

bool GetStateCell(Cell *c);
void SwitchStateCell(Cell *c);
void SetStateCell(Cell *c, bool val);

// Fonction World Display

Cell **GetTabOfWorlToDisplay(WorldToDisplay *world);
int GetSizeOfWorldToDisplay(WorldToDisplay *world);
WorldToDisplay *WorldToDisplayFromWorld(World *w);
void FreeWorldToDisplay(WorldToDisplay *wtd);

// Fonction World

Cell GetCellWorld(World *w, int x, int y);
int GetSizeWorld(World *w);
float GetZoomWorld(World *w);
int GetOffsetXWorld(World *w);
int GetOffsetYWorld(World *w);

Cell **InitCell2D(int size);
void FillCell2DToFalse(Cell **tab, int size);
void FillCell2DToRandom(Cell **tab, int size);

World *InitWorld(int size);

Cell **GetPerseption(World *w, int x, int y);
void DeletePerception(Cell **perception);

void IncrementZoom(World *w);
void DecrementZoom(World *w);

void IncrementOffsetX(World *w);
void DecrementOffsetX(World *w);

void IncrementOffsetY(World *w);
void DecrementOffsetY(World *w);

void DeleteWorld(World *w);

#endif
