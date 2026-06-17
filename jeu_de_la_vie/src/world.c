/**
 * \file world.c
 * \brief Implémentation des fonctions de gestion du monde.
 */

#include "world.h"

#define COEFF_ZOOM 0.1f

/**
 * \brief Crée et initialise un nouveau monde.
 * \param size Taille du monde (grille size x size).
 * \return Pointeur vers le monde créé.
 */
World *InitWorld(int size)
{
    World *world = calloc(size, sizeof(World));
    world->tab = InitCell2D(size);
    FillCell2DToFalse(world->tab, size);
    world->OffsetX = 0;
    world->OffsetY = 0;
    world->size = size;
    world->zoomDisplay = 1;
    return world;
}

/**
 * \brief Remplit le monde avec des cellules aléatoires (vivantes ou mortes).
 * \param w Pointeur vers le monde.
 */
void RandomizeWorld(World *w)
{
    for (int y = 0; y < w->size; y++)
    {
        for (int x = 0; x < w->size; x++)
        {
            w->tab[y][x].state = (rand() % 2 == 0);
        }
    }
}

/**
 * \brief Récupère une cellule aux coordonnées données (avec wrapping torique).
 * \param w Pointeur vers le monde.
 * \param x Coordonnée x.
 * \param y Coordonnée y.
 * \return La cellule aux coordonnées (x, y).
 */
Cell GetCellWorld(World *w, int x, int y)
{
    return w->tab[x % w->size][y % w->size];
}

/**
 * \brief Retourne la taille du monde.
 * \param w Pointeur vers le monde.
 * \return Taille du monde.
 */
int GetSizeWorld(World *w)
{
    return w->size;
}

/**
 * \brief Retourne le niveau de zoom actuel.
 * \param w Pointeur vers le monde.
 * \return Coefficient de zoom.
 */
float GetZoomWorld(World *w)
{
    return w->zoomDisplay;
}

/**
 * \brief Retourne le décalage horizontal de la vue.
 * \param w Pointeur vers le monde.
 * \return Décalage en x.
 */
int GetOffsetXWorld(World *w)
{
    return w->OffsetX;
}

/**
 * \brief Retourne le décalage vertical de la vue.
 * \param w Pointeur vers le monde.
 * \return Décalage en y.
 */
int GetOffsetYWorld(World *w)
{
    return w->OffsetY;
}

/**
 * \brief Récupère la perception 3x3 autour d'une cellule (voisinage de Moore).
 * \param w Pointeur vers le monde.
 * \param x Coordonnée x de la cellule centrale.
 * \param y Coordonnée y de la cellule centrale.
 * \return Tableau 3x3 de cellules (alloué dynamiquement, à libérer avec DeletePerception).
 */
Cell **GetPerseption(World *w, int x, int y)
{
    Cell **perception = calloc(3, sizeof(Cell *));
    if (!perception)
        return NULL;

    for (int i = 0; i < 3; i++)
    {
        perception[i] = calloc(3, sizeof(Cell));
        if (!perception[i])
        {
            for (int j = 0; j < i; j++)
                free(perception[j]);
            free(perception);
            return NULL;
        }

        for (int j = 0; j < 3; j++)
        {
            perception[i][j] = GetCellWorld(w, x - 1 + j, y - 1 + i);
        }
    }
    return perception;
}

/\*\*
 * \brief Libère la mémoire d'une perception 3x3.
 * \param perception Tableau 3x3 à libérer.
 */
void DeletePerception(Cell **perception)
{
    if (!perception)
        return;
    for (int i = 0; i < 3; i++)
    {
        free(perception[i]);
    }
    free(perception);
}

/**
 * \brief Augmente le zoom (rapproche la vue).
 * \param w Pointeur vers le monde.
 */
void IncrementZoom(World *w)
{
    w->zoomDisplay += COEFF_ZOOM;
    if (w->zoomDisplay > w->size)
        w->zoomDisplay = w->size;
}

/**
 * \brief Diminue le zoom (éloigne la vue).
 * \param w Pointeur vers le monde.
 */
void DecrementZoom(World *w)
{
    w->zoomDisplay -= COEFF_ZOOM;
    if (w->zoomDisplay < 1)
        w->zoomDisplay = 1;
}

/**
 * \brief Décale la vue vers la droite.
 * \param w Pointeur vers le monde.
 */
void IncrementOffsetX(World *w)
{
    w->OffsetX++;
}

/**
 * \brief Décale la vue vers la gauche.
 * \param w Pointeur vers le monde.
 */
void DecrementOffsetX(World *w)
{
    w->OffsetX--;
}

/**
 * \brief Décale la vue vers le bas.
 * \param w Pointeur vers le monde.
 */
void IncrementOffsetY(World *w)
{
    w->OffsetY++;
}

/**
 * \brief Décale la vue vers le haut.
 * \param w Pointeur vers le monde.
 */
void DecrementOffsetY(World *w)
{
    w->OffsetY--;
}

/**
 * \brief Libère la mémoire du monde et de sa grille.
 * \param w Pointeur vers le monde.
 */
void DeleteWorld(World *w)
{
    if (!w)
        return;
    for (int i = 0; i < w->size; i++)
    {
        free(w->tab[i]);
    }
    free(w->tab);
}

/**
 * \brief Affiche les informations du monde dans le terminal.
 * \param w Pointeur vers le monde.
 */
void PrintInfoWorld(World *w)
{
    printf("World : size=%d, zoom=%.2f, OffsetX=%d, OffsetY=%d\n", w->size, w->zoomDisplay, w->OffsetX, w->OffsetY);
}

/**
 * \brief Récupère le tableau de cellules d'un WorldToDisplay.
 * \param wtd Pointeur vers le WorldToDisplay.
 * \return Pointeur vers le tableau 2D de cellules.
 */
Cell **GetTabOfWorlToDisplay(WorldToDisplay *wtd)
{
    return wtd->tab;
}

/**
 * \brief Retourne la taille du WorldToDisplay.
 * \param wtd Pointeur vers le WorldToDisplay.
 * \return Taille du tableau.
 */
int GetSizeOfWorldToDisplay(WorldToDisplay *wtd)
{
    return wtd->size;
}

/**
 * \brief Crée un WorldToDisplay à partir d'un monde, en appliquant zoom et offset.
 * \param w Pointeur vers le monde source.
 * \return Pointeur vers le WorldToDisplay créé, ou NULL en cas d'échec.
 */
WorldToDisplay *WorldToDisplayFromWorld(World *w)
{
    WorldToDisplay *result = calloc(1, sizeof(WorldToDisplay));
    if (!result)
        return NULL;

    result->size = w->size / w->zoomDisplay;

    result->tab = calloc(result->size, sizeof(Cell *));
    if (!result->tab)
        return NULL;

    for (int i = 0; i < result->size; i++)
    {
        result->tab[i] = calloc(result->size, sizeof(Cell));
        if (!result->tab[i])
        {
            for (int j = 0; j < i; j++)
                free(result->tab[j]);
            free(result->tab);
            return NULL;
        }

        for (int j = 0; j < result->size; j++)
        {
            result->tab[i][j] = GetCellWorld(w, w->OffsetX + j, w->OffsetY + i);
        }
    }
    return result;
}

/**
 * \brief Libère la mémoire d'un WorldToDisplay.
 * \param wtd Pointeur vers le WorldToDisplay.
 */
void FreeWorldToDisplay(WorldToDisplay *wtd)
{
    if (!wtd)
        return;
    for (int i = 0; i < wtd->size; i++)
    {
        free(wtd->tab[i]);
    }
    free(wtd->tab);
}