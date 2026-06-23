#include "utils.h"

float random_float(float min, float max) {
    /**
     * @brief petite fonction auxiliaire pour déterminer un float dans une
     * range*/
    return min + ((float)rand()) / ((float)RAND_MAX) * (max - min);
}

void box_muller_standard(Vector *vect) {
    float u1 = random_float(0, 1);
    float u2 = random_float(0, 1);

    // Sécurité pour éviter log(0) qui tend vers l'infini -inf
    if (u2 < 1e-7f)
        u2 = 1e-7f;

    float t = 2.0f * PI * u1;
    float s = -2.0f * logf(u2);

    vect->x = sqrtf(s) * cosf(t);
    vect->y = sqrtf(s) * sinf(t);
}