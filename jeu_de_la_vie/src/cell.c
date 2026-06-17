/**
 * \file cell.c
 * \brief Implémentation des fonctions de gestion des cellules.
 */

#include "cell.h"

/**
 * \brief Vérifie si une cellule est vivante.
 * \param c Pointeur vers la cellule.
 * \return true si la cellule est vivante, false sinon.
 */
bool IsAlive(Cell *c)
{
    return c->state;
}

/**
 * \brief Bascule l'état d'une cellule (vivante <-> morte).
 * \param c Pointeur vers la cellule.
 */
void SwitchStateCell(Cell *c)
{
    c->state = !c->state;
}

/**
 * \brief Définit l'état d'une cellule.
 * \param c Pointeur vers la cellule.
 * \param val Nouvel état : true pour vivante, false pour morte.
 */
void SetStateCell(Cell *c, bool val)
{
    c->state = val;
}

/**
 * \brief Alloue un tableau 2D de cellules de taille donnée.
 * \param size Nombre de lignes et de colonnes.
 * \return Pointeur vers le tableau 2D alloué, ou NULL en cas d'échec.
 */
Cell **InitCell2D(int size)
{
    Cell **tab = calloc(size, sizeof(Cell *));
    for (int y = 0; y < size; ++y)
    {
        tab[y] = calloc(size, sizeof(Cell));
    }
    return tab;
}

/**
 * \brief Initialise toutes les cellules d'un tableau 2D à l'état mort.
 * \param tab Tableau 2D de cellules.
 * \param size Taille du tableau.
 */
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
