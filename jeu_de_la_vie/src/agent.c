/**
 * \file agent.c
 * \brief Implémentation des règles du Jeu de la Vie (voisinage et transitions).
 */

#include "../include/agent.h"

/**
 * \brief Compte le nombre de cellules dans la perception 3x3 (voisinage de Moore).
 * \param c Tableau 3x3 de cellules (perception autour d'une cellule).
 * \return Nombre de cellules comptées (9 pour une grille 3x3 complète).
 */
int GetNbNeighbors(Cell **c) {
    if (c == NULL) {
        printf("Error NULL in GetNbNeighbors");
        return 0;
    }

    int nbNeightbors = 0;
    int around = 3;

    for (int y = 0; y < around; ++y) {
        for (int x = 0; x < around; ++x) {
            if (c[x][y].state && (x != 1 || y != 1))
                ++nbNeightbors;
        }
    }
    return nbNeightbors;
}

/**
 * \brief Détermine le nouvel état d'une cellule selon les règles du Jeu de la Vie.
 *
 * Règles de Conway :
 * - Une cellule vivante survit si elle a 2 ou 3 voisins vivants.
 * - Une cellule morte naît si elle a exactement 3 voisins vivants.
 * - Sinon, la cellule meurt ou reste morte.
 *
 * \param c Pointeur vers la cellule courante.
 * \param nbNeightbors Nombre de voisins vivants.
 * \return true si la cellule sera vivante à l'itération suivante, false sinon.
 */
bool NewState(Cell *c, int nbNeightbors) {
    if (IsAlive(c)) {
        return nbNeightbors == 2 || nbNeightbors == 3;
    } else {
        return nbNeightbors == 3;
    }
}