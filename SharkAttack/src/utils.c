#include "utils.h"

__thread unsigned int seed = RANDOM_SEED;

int init_seed(unsigned int set_seed) {
    seed = set_seed;
    return seed;
}

int rand_trsf() {
    unsigned int next = seed;
    int result;

    next *= 1103515245;
    next += 12345;
    result = (unsigned int)(next / 65536) % 2048;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int)(next / 65536) % 1024;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int)(next / 65536) % 1024;

    seed = next;

    return result;
}

float random_float(float min, float max) {
    /**
     * @brief petite fonction auxiliaire pour déterminer un float dans une
     * range*/
    return min + ((float)rand_trsf()) / ((float)RAND_MAX) * (max - min);
}

/**
 * @brief Fonction permettant de tirer une variable aléatoire selon une loi
 * gaussienne.
 *
 * @return Vector Vecteur résultat du tirage aléatoire.
 */
Vector box_muller_standard() {
    Vector vect;
    float u1 = random_float(0, 1);
    float u2 = random_float(0, 1);

    // Sécurité pour éviter log(0) qui tend vers l'infini -inf
    if (u2 < 1e-7f)
        u2 = 1e-7f;

    float t = 2.0f * PI * u1;
    float s = -2.0f * logf(u2);

    vect.x = sqrtf(s) * cosf(t);
    vect.y = sqrtf(s) * sinf(t);
    return vect;
}
