#include "collider.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

// --- Helpers de comparaison ---
#define ASSERT_VEC(expected, actual, tol)                                     \
    do {                                                                       \
        TEST_ASSERT_FLOAT_EQUAL((expected).x, (actual).x, (tol));              \
        TEST_ASSERT_FLOAT_EQUAL((expected).y, (actual).y, (tol));              \
    } while (0)

// --- Tests Instantiate_collider ---

void test_Instantiate_collider_origin(void) {
    // Collider centré à l'origine, 10x10
    Vector pos = {0.0f, 0.0f};
    Collider c = Instantiate_collider(pos, 10.0f, 10.0f);

    // Coin supérieur gauche : (-5, -5)
    ASSERT_VEC((Vector){-5.0f, -5.0f}, c.bounding_box[0], 0.0001f);
    // Coin inférieur droit : (5, 5)
    ASSERT_VEC((Vector){5.0f, 5.0f}, c.bounding_box[1], 0.0001f);
}

void test_Instantiate_collider_offset_position(void) {
    // Collider centré en (20, 30), 8x6
    Vector pos = {20.0f, 30.0f};
    Collider c = Instantiate_collider(pos, 8.0f, 6.0f);

    ASSERT_VEC((Vector){16.0f, 27.0f}, c.bounding_box[0], 0.0001f);
    ASSERT_VEC((Vector){24.0f, 33.0f}, c.bounding_box[1], 0.0001f);
}

void test_Instantiate_collider_symmetric(void) {
    // La bounding box doit être symétrique autour de pos
    Vector pos = {50.0f, 80.0f};
    float w = 20.0f;
    float h = 12.0f;
    Collider c = Instantiate_collider(pos, w, h);

    // Le centre doit correspondre à pos
    Vector center = {
        (c.bounding_box[0].x + c.bounding_box[1].x) / 2.0f,
        (c.bounding_box[0].y + c.bounding_box[1].y) / 2.0f
    };
    ASSERT_VEC(pos, center, 0.0001f);

    // La largeur et hauteur doivent correspondre
    float box_w = c.bounding_box[1].x - c.bounding_box[0].x;
    float box_h = c.bounding_box[1].y - c.bounding_box[0].y;
    TEST_ASSERT_FLOAT_EQUAL(w, box_w, 0.0001f);
    TEST_ASSERT_FLOAT_EQUAL(h, box_h, 0.0001f);
}

void test_Instantiate_collider_unit_size(void) {
    // Collider 1x1 centré en (5, 5)
    Vector pos = {5.0f, 5.0f};
    Collider c = Instantiate_collider(pos, 1.0f, 1.0f);

    ASSERT_VEC((Vector){4.5f, 4.5f}, c.bounding_box[0], 0.0001f);
    ASSERT_VEC((Vector){5.5f, 5.5f}, c.bounding_box[1], 0.0001f);
}

void test_Instantiate_collider_large_size(void) {
    // Collider de la taille du monde (1400x1000) centré au milieu
    Vector pos = {700.0f, 500.0f};
    Collider c = Instantiate_collider(pos, 1400.0f, 1000.0f);

    ASSERT_VEC((Vector){0.0f, 0.0f}, c.bounding_box[0], 0.0001f);
    ASSERT_VEC((Vector){1400.0f, 1000.0f}, c.bounding_box[1], 0.0001f);
}

void test_Instantiate_collider_non_square(void) {
    // Collider rectangulaire large
    Vector pos = {100.0f, 200.0f};
    Collider c = Instantiate_collider(pos, 40.0f, 10.0f);

    ASSERT_VEC((Vector){80.0f, 195.0f}, c.bounding_box[0], 0.0001f);
    ASSERT_VEC((Vector){120.0f, 205.0f}, c.bounding_box[1], 0.0001f);
}

void test_Instantiate_collider_negative_position(void) {
    // Position négative (cas limite)
    Vector pos = {-10.0f, -20.0f};
    Collider c = Instantiate_collider(pos, 6.0f, 4.0f);

    ASSERT_VEC((Vector){-13.0f, -22.0f}, c.bounding_box[0], 0.0001f);
    ASSERT_VEC((Vector){-7.0f, -18.0f}, c.bounding_box[1], 0.0001f);
}

void test_Instantiate_collider_float_dimensions(void) {
    // Dimensions non entières
    Vector pos = {0.0f, 0.0f};
    Collider c = Instantiate_collider(pos, 3.5f, 7.25f);

    ASSERT_VEC((Vector){-1.75f, -3.625f}, c.bounding_box[0], 0.0001f);
    ASSERT_VEC((Vector){1.75f, 3.625f}, c.bounding_box[1], 0.0001f);
}

void test_Instantiate_collider_returns_copy(void) {
    // Vérifie que la valeur retournée est une copie (pas un pointeur)
    Vector pos = {10.0f, 20.0f};
    Collider c1 = Instantiate_collider(pos, 8.0f, 6.0f);
    Collider c2 = Instantiate_collider(pos, 8.0f, 6.0f);

    // Les deux colliders doivent avoir les mêmes valeurs
    ASSERT_VEC(c1.bounding_box[0], c2.bounding_box[0], 0.0001f);
    ASSERT_VEC(c1.bounding_box[1], c2.bounding_box[1], 0.0001f);

    // Mais modifier c1 ne doit pas affecter c2
    c1.bounding_box[0].x = 999.0f;
    TEST_ASSERT_FLOAT_EQUAL(6.0f, c2.bounding_box[0].x, 0.0001f);
}

// --- Point d'entrée Unity ---

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_Instantiate_collider_origin);
    RUN_TEST(test_Instantiate_collider_offset_position);
    RUN_TEST(test_Instantiate_collider_symmetric);
    RUN_TEST(test_Instantiate_collider_unit_size);
    RUN_TEST(test_Instantiate_collider_large_size);
    RUN_TEST(test_Instantiate_collider_non_square);
    RUN_TEST(test_Instantiate_collider_negative_position);
    RUN_TEST(test_Instantiate_collider_float_dimensions);
    RUN_TEST(test_Instantiate_collider_returns_copy);

    return UNITY_END();
}
