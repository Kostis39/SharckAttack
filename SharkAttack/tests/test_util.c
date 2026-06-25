#include "unity.h"
#include "utils.h"
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {}

void tearDown(void) {}

// --- Helpers ---

#define ASSERT_FLOAT(expected, actual, tol)                                     \
    TEST_ASSERT_FLOAT_WITHIN((tol), (expected), (actual))

// ============================================================================
//  rand_trsf — déterminisme et propriétés du LCG
// ============================================================================

// Même graine → même première valeur
void test_rand_trsf_same_seed_same_first_value(void) {
    init_seed(42);
    int a = rand_trsf();

    init_seed(42);
    int b = rand_trsf();

    TEST_ASSERT_EQUAL_INT(a, b);
}

// Même graine → même séquence sur plusieurs appels
void test_rand_trsf_same_seed_same_sequence(void) {
    init_seed(123);
    int seq1[10];
    for (int i = 0; i < 10; i++)
        seq1[i] = rand_trsf();

    init_seed(123);
    int seq2[10];
    for (int i = 0; i < 10; i++)
        seq2[i] = rand_trsf();

    for (int i = 0; i < 10; i++)
        TEST_ASSERT_EQUAL_INT(seq1[i], seq2[i]);
}

// Graines différentes → premières valeurs différentes (cas général)
void test_rand_trsf_different_seed_different_value(void) {
    init_seed(1);
    int a = rand_trsf();

    init_seed(2);
    int b = rand_trsf();

    TEST_ASSERT_NOT_EQUAL(a, b);
}

// La séquence ne reste pas constante (valeurs consécutives différentes)
void test_rand_trsf_consecutive_values_differ(void) {
    init_seed(42);
    int first = rand_trsf();
    int second = rand_trsf();
    int third = rand_trsf();

    // Il est théoriquement possible que deux valeurs consécutives soient
    // égales, mais avec la graine 42 et ce LCG c'est faux — on vérifie
    // qu'au moins une différence existe parmi les trois.
    TEST_ASSERT_TRUE(first != second || second != third);
}

// Après réinitialisation, la séquence repart à l'identique
void test_rand_trsf_reinit_resets_sequence(void) {
    init_seed(77);
    int a1 = rand_trsf();
    (void)rand_trsf(); // avance la séquence

    init_seed(77);
    int a2 = rand_trsf();
    // On ne vérifie que la première valeur après reinit
    TEST_ASSERT_EQUAL_INT(a1, a2);
}

// Graine 0 → ne plante pas, produit une valeur
void test_rand_trsf_zero_seed_does_not_crash(void) {
    init_seed(0);
    (void)rand_trsf();
    // On vérifie simplement que l'appel retourne sans erreur.
    TEST_ASSERT_TRUE(1);
}

// Graine maximale (UINT_MAX) → ne plante pas
void test_rand_trsf_max_seed_does_not_crash(void) {
    init_seed(0xFFFFFFFF);
    (void)rand_trsf();
    TEST_ASSERT_TRUE(1);
}

// ============================================================================
//  random_float — bornes et cohérence
// ============================================================================

// random_float retourne une valeur dans [min, max]
void test_random_float_within_bounds(void) {
    init_seed(42);
    for (int i = 0; i < 1000; i++) {
        float v = random_float(10.0f, 20.0f);
        TEST_ASSERT_TRUE(v >= 10.0f);
        TEST_ASSERT_TRUE(v <= 20.0f);
    }
}

// random_float avec min == max retourne min
void test_random_float_zero_range(void) {
    init_seed(42);
    float v = random_float(5.0f, 5.0f);
    ASSERT_FLOAT(5.0f, v, 0.0001f);
}

// random_float avec bornes négatives
void test_random_float_negative_range(void) {
    init_seed(42);
    for (int i = 0; i < 500; i++) {
        float v = random_float(-30.0f, -10.0f);
        TEST_ASSERT_TRUE(v >= -30.0f);
        TEST_ASSERT_TRUE(v <= -10.0f);
    }
}

// random_float avec bornes traversant zéro
void test_random_float_crossing_zero(void) {
    init_seed(42);
    for (int i = 0; i < 500; i++) {
        float v = random_float(-5.0f, 5.0f);
        TEST_ASSERT_TRUE(v >= -5.0f);
        TEST_ASSERT_TRUE(v <= 5.0f);
    }
}

// Même graine → même séquence de random_float
void test_random_float_same_seed_same_sequence(void) {
    init_seed(99);
    float seq1[5];
    for (int i = 0; i < 5; i++)
        seq1[i] = random_float(0.0f, 1.0f);

    init_seed(99);
    for (int i = 0; i < 5; i++) {
        float v = random_float(0.0f, 1.0f);
        ASSERT_FLOAT(seq1[i], v, 0.0001f);
    }
}

// random_float produit des valeurs variées (pas toutes identiques)
void test_random_float_produces_varied_values(void) {
    init_seed(42);
    float first = random_float(0.0f, 1.0f);
    int all_same = 1;
    for (int i = 0; i < 100; i++) {
        float v = random_float(0.0f, 1.0f);
        if (fabsf(v - first) > 0.001f) {
            all_same = 0;
            break;
        }
    }
    TEST_ASSERT_FALSE(all_same);
}

// ============================================================================
//  box_muller_standard — propriétés statistiques
// ============================================================================

// box_muller ne retourne pas (0,0) de manière systématique
void test_box_muller_not_always_zero(void) {
    init_seed(42);
    int non_zero = 0;
    for (int i = 0; i < 100; i++) {
        Vector v = box_muller_standard();
        if (fabsf(v.x) > 0.001f || fabsf(v.y) > 0.001f)
            non_zero++;
    }
    TEST_ASSERT_TRUE(non_zero > 50);
}

// box_muller : les valeurs ne divergent pas (pas d'Inf/NaN)
void test_box_muller_no_nan_or_inf(void) {
    init_seed(42);
    for (int i = 0; i < 1000; i++) {
        Vector v = box_muller_standard();
        TEST_ASSERT_FALSE(isnan(v.x));
        TEST_ASSERT_FALSE(isnan(v.y));
        TEST_ASSERT_FALSE(isinf(v.x));
        TEST_ASSERT_FALSE(isinf(v.y));
    }
}

// box_muller : même graine → même séquence
void test_box_muller_same_seed_same_sequence(void) {
    init_seed(42);
    Vector seq1[5];
    for (int i = 0; i < 5; i++)
        seq1[i] = box_muller_standard();

    init_seed(42);
    for (int i = 0; i < 5; i++) {
        Vector v = box_muller_standard();
        ASSERT_FLOAT(seq1[i].x, v.x, 0.0001f);
        ASSERT_FLOAT(seq1[i].y, v.y, 0.0001f);
    }
}

// box_muller : moyenne empirique proche de 0 (test statistique souple)
void test_box_muller_mean_near_zero(void) {
    init_seed(42);
    int n = 10000;
    float sum_x = 0.0f, sum_y = 0.0f;
    for (int i = 0; i < n; i++) {
        Vector v = box_muller_standard();
        sum_x += v.x;
        sum_y += v.y;
    }
    float mean_x = sum_x / n;
    float mean_y = sum_y / n;
    // Avec 10 000 échantillons N(0,1), la moyenne doit être dans [-0.1, 0.1]
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, mean_x);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, mean_y);
}

// box_muller : variance empirique proche de 1
void test_box_muller_variance_near_one(void) {
    init_seed(42);
    int n = 10000;
    float sum_x2 = 0.0f, sum_y2 = 0.0f;
    for (int i = 0; i < n; i++) {
        Vector v = box_muller_standard();
        sum_x2 += v.x * v.x;
        sum_y2 += v.y * v.y;
    }
    float var_x = sum_x2 / n;
    float var_y = sum_y2 / n;
    TEST_ASSERT_FLOAT_WITHIN(0.15f, 1.0f, var_x);
    TEST_ASSERT_FLOAT_WITHIN(0.15f, 1.0f, var_y);
}

// box_muller : les deux composantes sont indépendantes (corrélation faible)
void test_box_muller_components_uncorrelated(void) {
    init_seed(42);
    int n = 10000;
    float sum_xy = 0.0f;
    for (int i = 0; i < n; i++) {
        Vector v = box_muller_standard();
        sum_xy += v.x * v.y;
    }
    float corr = sum_xy / n; // E[XY] ≈ 0 si indépendantes (moyenne ≈ 0)
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, corr);
}

// ============================================================================
//  Point d'entrée Unity
// ============================================================================

int main(void) {
    UNITY_BEGIN();

    // --- rand_trsf ---
    RUN_TEST(test_rand_trsf_same_seed_same_first_value);
    RUN_TEST(test_rand_trsf_same_seed_same_sequence);
    RUN_TEST(test_rand_trsf_different_seed_different_value);
    RUN_TEST(test_rand_trsf_consecutive_values_differ);
    RUN_TEST(test_rand_trsf_reinit_resets_sequence);
    RUN_TEST(test_rand_trsf_zero_seed_does_not_crash);
    RUN_TEST(test_rand_trsf_max_seed_does_not_crash);

    // --- random_float ---
    RUN_TEST(test_random_float_within_bounds);
    RUN_TEST(test_random_float_zero_range);
    RUN_TEST(test_random_float_negative_range);
    RUN_TEST(test_random_float_crossing_zero);
    RUN_TEST(test_random_float_same_seed_same_sequence);
    RUN_TEST(test_random_float_produces_varied_values);

    // --- box_muller_standard ---
    RUN_TEST(test_box_muller_not_always_zero);
    RUN_TEST(test_box_muller_no_nan_or_inf);
    RUN_TEST(test_box_muller_same_seed_same_sequence);
    RUN_TEST(test_box_muller_mean_near_zero);
    RUN_TEST(test_box_muller_variance_near_one);
    RUN_TEST(test_box_muller_components_uncorrelated);

    return UNITY_END();
}
