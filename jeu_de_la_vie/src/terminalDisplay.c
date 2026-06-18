/**
 * \file terminalDisplay.c
 * \brief Implémentation de l'affichage terminal (mode texte) du Jeu de la Vie.
 */

#include "terminalDisplay.h"

/**
 * \brief Affiche le monde dans le terminal avec des codes ANSI.
 *
 * Les cellules vivantes sont affichées en blanc (\033[47m) et
 * les cellules mortes en noir (\033[40m).
 *
 * \param monde Pointeur vers le WorldToDisplay à afficher.
 * \return 0 en cas de succès.
 */
int Display(WorldToDisplay *monde) {
    for (int y = 0; y < monde->size; ++y) {
        for (int x = 0; x < monde->size; ++x) {
            if (monde->tab[x][y].state) {
                printf("\033[47m  ");
            } else {
                printf("\033[40m  ");
            }
        }
        printf("\033[0m\n");
    }
    return 0;
}
