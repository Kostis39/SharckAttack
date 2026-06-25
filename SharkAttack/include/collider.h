#ifndef COLLIDER_H
#define COLLIDER_H

#include "string.h"
#include "utils.h"
#include "vector.h"
#include <stdbool.h>

typedef struct {
    Vector bounding_box[2]; /**< Bounding box de l'objet de collision, coin
                               supérieur gauche et coin inférieur droit*/
    /* bool is_enabled; */
} Collider;

/**
 * @brief Fonction d'instanciation d'objet de collision
 * Crée un rectangle de collision de largeur w, de hauteur h et de
 * barycentre pos
 * @param pos vecteur du centre
 * @param w largeur
 * @param h hauteur
 * @return new_collider instancié
 * */
Collider Instantiate_collider(Vector pos, float w, float h);

/**
 * @brief initialise un tableau de colliders aléatoirement positionnés
 * @param seed graine de l'initialisation aléatoire
 * @param x_max borne supérieure de la position (axe x)
 * @param y_max borne supérieure de la position (axe y)
 * on assume que la borne inférieure est toujours le (0,0)
 * @param w_max borne supérieure de la largeur
 * @param h_max borne supérieure de la longueur
 * @param count nombre d'obstacles instanciés
 * */
Collider *Colliders_random_array(int x_max, int y_max, int w_max, int h_max,
                                 int count, int gen_all_squared);

/**
 * @brief Retourne le point le plus proche du rectangle (AABB) au point p
 * @param c le collider (bounding box)
 * @param p le point de référence
 * @return le point sur (ou dans) le rectangle le plus proche de p
 */
Vector Collider_closest_point(Collider *c, Vector p);

/**
 * @brief libère la mémoire utilisée par les colliders
 * @return 0 si réussite, 1 sinon
 * */
int Colliders_destroy_array(Collider *array);

/**
 * @brief Distance euclidienne entre un point et un rectangle (AABB)
 * @param c le collider
 * @param p le point
 * @return la distance (0 si le point est à l'intérieur)
 */
float Collider_distance_to_point(Collider *c, Vector p);

Collider *Colliders_copy_array(Collider *collider, int count);

#endif