#include "vector.h"

void Vector_print(Vector v) { printf("(%f, %f)\n", v.x, v.y); }
void VectorRule_print(VectorRule v) {
    printf("(");
    for (int i = 0; i < Rules_Lenght; ++i) {
        printf("%f", v.vect[i]);

        if (i < Rules_Lenght - 1) {
            printf(", ");
        }
    }
    printf(")");
}

Vector Vector_init(void) { return (Vector){0.0f, 0.0f}; }
VectorRule VectorRule_init(void) {
    VectorRule rules;
    for (int i = 0; i < Rules_Lenght; ++i) {
        rules.vect[i] = 0.0f;
    }
    return rules;
}

Vector Vector_add(Vector a, Vector b) { return (Vector){a.x + b.x, a.y + b.y}; }

Vector Vector_sub(Vector a, Vector b) { return (Vector){a.x - b.x, a.y - b.y}; }

Vector Vector_scale(Vector v, float k) { return (Vector){v.x * k, v.y * k}; }

float Vector_length(Vector v) { return sqrtf(v.x * v.x + v.y * v.y); }
float Vector_length2(Vector v) {
    /** @brief renvoie la distance au carré*/
    return v.x * v.x + v.y * v.y;
}
float Vector_distance(Vector a, Vector b) {
    return Vector_length(Vector_sub(a, b));
}

Vector Vector_normalize(Vector v) {
    float len = Vector_length(v);
    if (len == 0.0f)
        return v;
    return Vector_scale(v, 1.0f / len);
}

Vector local_normalize(Vector v) {
    float len = Vector_length(v);
    if (len < 0.0001f) {
        Vector default_vector = {1.0f, 0.0f};
        return default_vector;
    }
    return Vector_scale(v, 1.0f / len);
}

/**
 * @brief Fonction qui limite la longueur d'un vecteur à une valeur maximale
 * donnée. Si la longueur du vecteur dépasse cette valeur, le vecteur est
 * normalisé et mis à l'échelle pour correspondre à la longueur maximale. Si
 * la longueur du vecteur est inférieure ou égale à la longueur maximale, le
 * vecteur est retourné inchangé.
 *
 * @param v Le vecteur à limiter
 * @param max_length La longueur maximale autorisée pour le vecteur
 * @return Vector Le vecteur limité à la longueur maximale spécifiée
 */
Vector Vector_limit(Vector v, float max_length) {
    float len = Vector_length(v);
    if (len > max_length)
        return Vector_scale(Vector_normalize(v), max_length);
    return v;
}

float fast_atan2f(float y, float x) {
    if (x == 0.0f && y == 0.0f)
        return 0.0f;

    float abs_y = fabs(y);
    float abs_x = fabs(x);
    float angle;

    if (abs_x >= abs_y) {
        float r = y / x;
        // Approximation par polynôme de degré 3
        angle = r * (0.785398163f -
                     (abs_x - abs_y) / (abs_x + abs_y) * 0.214601836f);
    } else {
        float r = x / y;
        float sign = (y < 0.0f) ? -1.0f : 1.0f;
        angle = sign * 1.570796327f -
                r * (0.785398163f -
                     (abs_y - abs_x) / (abs_x + abs_y) * 0.214601836f);
    }
    return angle;
}

float Vector_angle(Vector a, Vector b) {
    // Produit scalaire : a · b = |a||b|cos(θ)
    float dot = a.x * b.x + a.y * b.y;

    // Produit vectoriel (z seulement en 2D) : a × b = |a||b|sin(θ)
    float cross = a.x * b.y - a.y * b.x;

    // atan2(sin, cos) donne l'angle signé dans [-π, π]
    return atan2f(cross, dot);
}

float Vector_angle_fast(Vector a, Vector b) {
    /**
     * @brief Calcule l'angle entre deux vecteurs avec une approximation
     * d'Arctangente
     * @note Moins précis, plus rapide, à utiliser sur les poissons par
     * exemple
     * @param a premier vecteur
     * @param b second vecteur
     * @return L'angle absolu a,b
     */
    float dot = a.x * b.x + a.y * b.y;
    float cross = a.x * b.y - a.y * b.x;
    return fast_atan2f(cross, dot);
}
