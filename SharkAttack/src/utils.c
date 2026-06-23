#include "utils.h"

float random_float(float min, float max) {
    /**
     * @brief petite fonction auxiliaire pour déterminer un float dans une
     * range*/
    return min + ((float)rand()) / ((float)RAND_MAX) * (max - min);
}

void box_muller_standard(Vector *vect) {
    // 1. rand() / RAND_MAX donne un nombre uniforme entre 0 et 1 (noté rand(1)
    // dans l'algo)
    float u1 = random_float(0, 1);
    float u2 = random_float(0, 1);

    // Sécurité pour éviter log(0) qui tend vers l'infini -inf
    if (u1 < 1e-7f)
        u1 = 1e-7f;

    // 2. Application directe de l'algorithme fourni
    float t = 2.0f * (float)PI * u2; // Thêta : uniforme sur [0, 2π]
    float s = -2.0f * logf(u1);      // S = R^2 : suit une loi exponentielle

    // 3. Transformation en coordonnées cartésiennes pour obtenir X et Y
    vect->x = sqrtf(s) * cosf(t);
    vect->y = sqrtf(s) * sinf(t);
}