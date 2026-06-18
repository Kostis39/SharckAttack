/**
 * \file mj.c
 * \brief Implémentation de la mise à jour du monde (itération du jeu).
 */

#include "mj.h"

/**
 * \brief Copie l'état du monde temporaire vers le monde principal, puis
 * réinitialise le temporaire.
 *
 * Cette fonction réalise un double-buffering : les nouveaux états sont calculés
 * dans worldTmp puis appliqués à worldNow. Le tableau temporaire est ensuite
 * remis à zéro pour l'itération suivante.
 *
 * \param worldNow Pointeur vers le monde principal.
 * \param worldTmp Tableau temporaire contenant les nouveaux états.
 */
void SwitchTabCellWorld(World *worldNow, Cell **worldTmp) {
    if (worldNow == NULL || worldNow->tab == NULL || worldTmp == NULL)
        return;

    int size = worldNow->size;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            SetStateCell(&worldNow->tab[x][y], IsAlive(&worldTmp[x][y]));
            SetStateCell(&worldTmp[x][y], false);
        }
    }
}

/**
 * \brief Définit l'état d'une cellule dans un tableau 2D.
 * \param tab Tableau 2D de cellules.
 * \param x Coordonnée x.
 * \param y Coordonnée y.
 * \param val Nouvel état : true pour vivante, false pour morte.
 */
void SetValueTabCell(Cell **tab, int x, int y, bool val) {
    SetStateCell(&tab[x][y], val);
}
