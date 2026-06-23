#ifndef SHARK_H
#define SHARK_H

#include "config.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * @struct Shark
 * @brief Structure représentant le requin dans le jeu.
 */
typedef struct {
    Vector pos;      /**< Position du requin dans le monde */
    Vector velocity; /**< Vecteur direction/vitesse du requin dans le monde */
    float radius_vision; /**< Rayon de vision du requin pour détecter les
                            poissons */
    bool player;         // true le requin est un joueur, false un bot
} Shark;

/**
 * @brief Structure représentant le résultat des règles, nommée phi dans le
 * cours.
 */
typedef struct {
    Vector center;    /**< Vecteur résultat de la règle center */
    Vector alignment; /**< Vecteur résultat de la règle alignement */
    Vector pursuit;   /**< Vecteur résultat de la règle pursuit */
} SharkPhi;

Shark *Shark_create(int width, int height);
int Shark_copy(Shark *shark_dest, Shark *shark_src);
void Shark_destroy(Shark *shark);

bool Is_player(Shark *shark);

void Shark_apply_action(Shark *shark, Vector action);

#endif
