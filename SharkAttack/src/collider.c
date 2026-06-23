#include "collider.h"
#include "vector.h"
#include <stdlib.h>
#include <string.h>

Collider Instantiate_collider(Vector pos, float w, float h) {
    /**
     * @brief Fonction d'instanciation d'objet de collision
     * Crée un rectangle de collision de largeur w, de hauteur h et de
     * barycentre pos
     * @param pos vecteur du centre
     * @param w largeur
     * @param h hauteur
     * @return new_collider instancié
     * */
    Collider new_collider;
    new_collider.bounding_box[0].x = pos.x - w / 2;
    new_collider.bounding_box[0].y = pos.y - h / 2;

    new_collider.bounding_box[1].x = pos.x + w / 2;
    new_collider.bounding_box[1].y = pos.y + h / 2;
    return new_collider;
}

Collider *Colliders_random_array(int x_max, int y_max, int w_max, int h_max,
                                 int count) {
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
    Collider *array = calloc(count, sizeof(*array));
    if (!array) {
        return NULL;
    }
    Vector current_pos = Vector_init();
    float cur_w, cur_h;
    for (int i = 0; i < count; ++i) {
        cur_w = random_float(0, w_max);
        cur_h = random_float(0, h_max);

        // generer le centre
        current_pos.x = random_float(0, x_max);
        current_pos.y = random_float(0, y_max);

        array[i] = Instantiate_collider(current_pos, cur_w,
                                        cur_w); // collider d'indice i
    }
    return array;
}

int Colliders_destroy_array(Collider *array) {
    /**
     * @brief libère la mémoire utilisée par les colliders
     * @return 0 si réussite, 1 sinon
     * */
    if (array) {
        free(array);
        return 0;
    }
    return -1;
}

Vector Collider_closest_point(Collider *c, Vector p) {
    /**
     * @brief Retourne le point le plus proche du rectangle (AABB) au point p
     * @param c le collider (bounding box)
     * @param p le point de référence
     * @return le point sur (ou dans) le rectangle le plus proche de p
     */
    Vector closest;
    closest.x = fmaxf(c->bounding_box[0].x,
                      fminf(p.x, c->bounding_box[1].x));
    closest.y = fmaxf(c->bounding_box[0].y,
                      fminf(p.y, c->bounding_box[1].y));
    return closest;
}

float Collider_distance_to_point(Collider *c, Vector p) {
    /**
     * @brief Distance euclidienne entre un point et un rectangle (AABB)
     * @param c le collider
     * @param p le point
     * @return la distance (0 si le point est à l'intérieur)
     */
    Vector closest = Collider_closest_point(c, p);
    return Vector_distance(p, closest);
}

Collider *Colliders_copy_array(Collider *collider, int count) {
    Collider *copy = malloc(count * sizeof(*collider));
    if (!(collider && copy)) {
        return NULL;
    }
    memcpy(copy, collider, count * sizeof(*collider));
    return copy;
}
