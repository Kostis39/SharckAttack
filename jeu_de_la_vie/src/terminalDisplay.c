#include "../include/terminalDisplay.h"
int Display(World *monde) {
  /**
   * @brief Affiche le monde selon ses paramètres
   * Affichage du monde en version terminale
   * @param monde Le monde à afficher.
   * @return 0 si succès
   * @return -1 si le monde n'entre pas en terminal
   */

  for (int i; i < monde->size; ++i) {
    for (int j; j < monde->size; ++j) {
      if (monde->tab[i][j].state) {
        printf("\033[30;107m\u2588\u2588");
      } else {

        printf("\033[30;40m\u2588\u2588");
      }
    }
    printf("\n");
  }
  return 0;
}
