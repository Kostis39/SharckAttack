#include "collider.h"
#include "vector.h"
#include <stdlib.h>

float random_float(float min, float max){
    /**
     * @brief petite fonction auxiliaire pour déterminer un float dans une
     * range*/
    return min + ((float)rand()) / ((float)RAND_MAX) * (max - min)}

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

Collider *Colliders_random_array(int seed, int x_max, int y_max, int w_max,
                                 int h_max, int count) {
    Collider *array = calloc(count, sizeof(*array));
    if (!array) {
        return NULL;
    }
    Vector current_pos = Vector_init();
    int cur_w, cur_h;
    srand(seed);
    for (int i = 0; i < count; ++i) {
        cur_w = random_float(0, w_max);
        cur_h = random_float(0, h_max);

        // generer le centre
        current_pos.x = random_float(0, x_max);
        current_pos.y = random_float(0, y_max);

        array[i] = Instantiate_collider(current_pos, cur_w,
                                        cur_h); // collider d'indice i
    }
    return array;
}
