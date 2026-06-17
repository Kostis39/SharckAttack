#include "terminalDisplay.h"
int Display(WorldToDisplay *monde)
{
  /**
   * @brief Affiche le monde selon ses paramètres
   * Affichage du monde en version terminale
   * @param monde Le monde à afficher.
   * @return 0 si succès
   * @return -1 si le monde n'entre pas en terminal
   */

  for (int y = 0; y < monde->size; ++y)
  {
    for (int x = 0; x < monde->size; ++x)
    {
      if (monde->tab[x][y].state)
      {
        printf("\033[47m  ");
      }
      else
      {

        printf("\033[40m  ");
      }
    }
    printf("\033[0m\n");
  }
  return 0;
}
