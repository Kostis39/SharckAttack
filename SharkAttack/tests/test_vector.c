#include "unity.h"
#include "vector.h"
#include <math.h>

void setUp(void) {}

void tearDown(void) {}

// --- Helpers ---
// Composante par composante avec tolerance, via des float intermediaires
// pour eviter que les parentheses de (Vector){...} confondent le preprocesseur.

#define ASSERT_FLOAT(expected, actual, tol)                                     \
    TEST_ASSERT_FLOAT_WITHIN((tol), (expected), (actual))

// --- Tests Vector_init ---

void test_Vector_init_returns_zero_vector(void) {
    Vector v = Vector_init();
    ASSERT_FLOAT(0.0f, v.x, 0.0001f);
    ASSERT_FLOAT(0.0f, v.y, 0.0001f);
}

// --- Tests Vector_add ---

void test_Vector_add_simple(void) {
    Vector a = {1.0f, 2.0f};
    Vector b = {3.0f, 4.0f};
    Vector result = Vector_add(a, b);
    ASSERT_FLOAT(4.0f, result.x, 0.0001f);
    ASSERT_FLOAT(6.0f, result.y, 0.0001f);
}

void test_Vector_add_with_negative(void) {
    Vector a = {-1.0f, -2.0f};
    Vector b = {3.0f, 4.0f};
    Vector result = Vector_add(a, b);
    ASSERT_FLOAT(2.0f, result.x, 0.0001f);
    ASSERT_FLOAT(2.0f, result.y, 0.0001f);
}

void test_Vector_add_with_zero(void) {
    Vector a = {5.0f, 7.0f};
    Vector b = {0.0f, 0.0f};
    Vector result = Vector_add(a, b);
    ASSERT_FLOAT(5.0f, result.x, 0.0001f);
    ASSERT_FLOAT(7.0f, result.y, 0.0001f);
}

// --- Tests Vector_sub ---

void test_Vector_sub_simple(void) {
    Vector a = {5.0f, 7.0f};
    Vector b = {2.0f, 3.0f};
    Vector result = Vector_sub(a, b);
    ASSERT_FLOAT(3.0f, result.x, 0.0001f);
    ASSERT_FLOAT(4.0f, result.y, 0.0001f);
}

void test_Vector_sub_same_vector_returns_zero(void) {
    Vector a = {3.0f, 4.0f};
    Vector result = Vector_sub(a, a);
    ASSERT_FLOAT(0.0f, result.x, 0.0001f);
    ASSERT_FLOAT(0.0f, result.y, 0.0001f);
}

// --- Tests Vector_scale ---

void test_Vector_scale_by_positive(void) {
    Vector v = {3.0f, 4.0f};
    Vector result = Vector_scale(v, 2.0f);
    ASSERT_FLOAT(6.0f, result.x, 0.0001f);
    ASSERT_FLOAT(8.0f, result.y, 0.0001f);
}

void test_Vector_scale_by_zero(void) {
    Vector v = {3.0f, 4.0f};
    Vector result = Vector_scale(v, 0.0f);
    ASSERT_FLOAT(0.0f, result.x, 0.0001f);
    ASSERT_FLOAT(0.0f, result.y, 0.0001f);
}

void test_Vector_scale_by_negative(void) {
    Vector v = {3.0f, 4.0f};
    Vector result = Vector_scale(v, -1.0f);
    ASSERT_FLOAT(-3.0f, result.x, 0.0001f);
    ASSERT_FLOAT(-4.0f, result.y, 0.0001f);
}

void test_Vector_scale_by_fraction(void) {
    Vector v = {10.0f, 20.0f};
    Vector result = Vector_scale(v, 0.5f);
    ASSERT_FLOAT(5.0f, result.x, 0.0001f);
    ASSERT_FLOAT(10.0f, result.y, 0.0001f);
}

// --- Tests Vector_length ---

void test_Vector_length_unit_vector(void) {
    Vector v = {1.0f, 0.0f};
    float len = Vector_length(v);
    ASSERT_FLOAT(1.0f, len, 0.0001f);
}

void test_Vector_length_pythagorean(void) {
    Vector v = {3.0f, 4.0f};
    float len = Vector_length(v);
    ASSERT_FLOAT(5.0f, len, 0.0001f);
}

void test_Vector_length_zero_vector(void) {
    Vector v = {0.0f, 0.0f};
    float len = Vector_length(v);
    ASSERT_FLOAT(0.0f, len, 0.0001f);
}

// --- Tests Vector_distance ---

void test_Vector_distance_same_point(void) {
    Vector a = {3.0f, 4.0f};
    float dist = Vector_distance(a, a);
    ASSERT_FLOAT(0.0f, dist, 0.0001f);
}

void test_Vector_distance_known(void) {
    Vector a = {0.0f, 0.0f};
    Vector b = {3.0f, 4.0f};
    float dist = Vector_distance(a, b);
    ASSERT_FLOAT(5.0f, dist, 0.0001f);
}

// --- Tests Vector_normalize ---

void test_Vector_normalize_unit_length(void) {
    Vector v = {3.0f, 4.0f};
    Vector n = Vector_normalize(v);
    float len = Vector_length(n);
    ASSERT_FLOAT(1.0f, len, 0.0001f);
    ASSERT_FLOAT(0.6f, n.x, 0.0001f);
    ASSERT_FLOAT(0.8f, n.y, 0.0001f);
}

void test_Vector_normalize_zero_vector_returns_zero(void) {
    Vector v = {0.0f, 0.0f};
    Vector n = Vector_normalize(v);
    // Le contrat : vecteur nul renvoye inchange (pas de division par zero)
    ASSERT_FLOAT(0.0f, n.x, 0.0001f);
    ASSERT_FLOAT(0.0f, n.y, 0.0001f);
}

// --- Tests Vector_limit ---

void test_Vector_limit_below_max(void) {
    Vector v = {3.0f, 4.0f}; // longueur = 5
    Vector result = Vector_limit(v, 10.0f);
    ASSERT_FLOAT(3.0f, result.x, 0.0001f);
    ASSERT_FLOAT(4.0f, result.y, 0.0001f);
}

void test_Vector_limit_above_max(void) {
    Vector v = {3.0f, 4.0f}; // longueur = 5
    Vector result = Vector_limit(v, 2.5f);
    float len = Vector_length(result);
    ASSERT_FLOAT(2.5f, len, 0.0001f);
    ASSERT_FLOAT(1.5f, result.x, 0.0001f);
    ASSERT_FLOAT(2.0f, result.y, 0.0001f);
}

void test_Vector_limit_exact_max(void) {
    Vector v = {3.0f, 4.0f}; // longueur = 5
    Vector result = Vector_limit(v, 5.0f);
    ASSERT_FLOAT(3.0f, result.x, 0.0001f);
    ASSERT_FLOAT(4.0f, result.y, 0.0001f);
}

// --- Tests Vector_angle ---

void test_Vector_angle_aligned(void) {
    Vector a = {1.0f, 0.0f};
    Vector b = {2.0f, 0.0f};
    float angle = Vector_angle(a, b);
    ASSERT_FLOAT(0.0f, angle, 0.0001f);
}

void test_Vector_angle_perpendicular(void) {
    Vector a = {1.0f, 0.0f};
    Vector b = {0.0f, 1.0f};
    float angle = Vector_angle(a, b);
    ASSERT_FLOAT(M_PI / 2.0f, angle, 0.0001f);
}

void test_Vector_angle_opposite(void) {
    Vector a = {1.0f, 0.0f};
    Vector b = {-1.0f, 0.0f};
    float angle = Vector_angle(a, b);
    ASSERT_FLOAT(M_PI, angle, 0.0001f);
}

// --- Point d'entree Unity ---

int main(void) {
    UNITY_BEGIN();

    // Vector_init
    RUN_TEST(test_Vector_init_returns_zero_vector);

    // Vector_add
    RUN_TEST(test_Vector_add_simple);
    RUN_TEST(test_Vector_add_with_negative);
    RUN_TEST(test_Vector_add_with_zero);

    // Vector_sub
    RUN_TEST(test_Vector_sub_simple);
    RUN_TEST(test_Vector_sub_same_vector_returns_zero);

    // Vector_scale
    RUN_TEST(test_Vector_scale_by_positive);
    RUN_TEST(test_Vector_scale_by_zero);
    RUN_TEST(test_Vector_scale_by_negative);
    RUN_TEST(test_Vector_scale_by_fraction);

    // Vector_length
    RUN_TEST(test_Vector_length_unit_vector);
    RUN_TEST(test_Vector_length_pythagorean);
    RUN_TEST(test_Vector_length_zero_vector);

    // Vector_distance
    RUN_TEST(test_Vector_distance_same_point);
    RUN_TEST(test_Vector_distance_known);

    // Vector_normalize
    RUN_TEST(test_Vector_normalize_unit_length);
    RUN_TEST(test_Vector_normalize_zero_vector_returns_zero);

    // Vector_limit
    RUN_TEST(test_Vector_limit_below_max);
    RUN_TEST(test_Vector_limit_above_max);
    RUN_TEST(test_Vector_limit_exact_max);

    // Vector_angle
    RUN_TEST(test_Vector_angle_aligned);
    RUN_TEST(test_Vector_angle_perpendicular);
    RUN_TEST(test_Vector_angle_opposite);

    return UNITY_END();
}
