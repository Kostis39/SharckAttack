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
    bool player;         /**< true le requin est un joueur, false un bot*/
} Shark;

/**
 * @brief crée un requin avec une position initiale et un mode joueur ou bot
 * @param width largeur du monde
 * @param height hauteur du monde
 * @param is_player true si le requin est contrôlé par le joueur, false pour un
 * bot
 * @return pointeur vers le requin alloué dynamiquement
 */
Shark *Shark_create(int width, int height, bool is_player);

/**
 * @brief copie les données d'un requin source vers un requin destination
 * @param shark_dest pointeur vers le requin de destination
 * @param shark_src pointeur vers le requin source
 * @return 0 si succès, -1 sinon
 */
int Shark_copy(Shark *shark_dest, Shark *shark_src);

/**
 * @brief libère la mémoire d'un requin
 * @param shark pointeur vers le requin à libérer
 */
void Shark_destroy(Shark *shark);

/**
 * @brief vérifie si le requin est contrôlé par le joueur
 * @param shark pointeur vers le requin
 * @return true si le requin est un joueur, false sinon
 */
bool Is_player(Shark *shark);

/**
 * @brief applique une action au requin et met à jour sa position
 * @param shark pointeur vers le requin
 * @param action vecteur vitesse à appliquer
 * @param width largeur du monde
 * @param height hauteur du monde
 */
void Shark_apply_action(Shark *shark, Vector action, int width, int height);

#endif
