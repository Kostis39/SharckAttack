#include "vector.h"
#include "config.h"

void Vector_print(Vector v) { printf("(%f, %f)\n", v.x, v.y); }

Vector Vector_init(void) { return (Vector){0.0f, 0.0f}; }

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
 * normalisé et mis à l'échelle pour correspondre à la longueur maximale. Si la
 * longueur du vecteur est inférieure ou égale à la longueur maximale, le
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

float Vector_angle(Vector a, Vector b) {
    // Produit scalaire : a · b = |a||b|cos(θ)
    float dot = a.x * b.x + a.y * b.y;

    // Produit vectoriel (z seulement en 2D) : a × b = |a||b|sin(θ)
    float cross = a.x * b.y - a.y * b.x;

    // atan2(sin, cos) donne l'angle signé dans [-π, π]
    return atan2f(cross, dot);
}
