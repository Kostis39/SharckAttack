#ifndef CELL_H
#define CELL_H

/**
 * \file cell.h
 * \brief Définition de la structure Cell et des fonctions associées.
 */

#include <stdbool.h>
#include <stdlib.h>

/**
 * \struct Cell
 * \brief Représente une cellule, elle est soit vivante soit morte.
 */
typedef struct {
    bool state; /**< Staut de la cellule, True: vivant, False: mort. */
} Cell;

bool IsAlive(Cell *c);
void SwitchStateCell(Cell *c);
void SetStateCell(Cell *c, bool val);

Cell **InitCell2D(int size);
void FillCell2DToFalse(Cell **tab, int size);
void FillCell2DToRandom(Cell **tab, int size);

void DeleteCell2D(Cell **tab, int size);

#endif