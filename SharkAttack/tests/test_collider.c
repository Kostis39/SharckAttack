#include "collider.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

// --- Helpers ---
// Comparaison de vecteurs composante par composante
// On evite les macros avec (Vector){...} qui confondent le preprocesseur

static void assert_vec(float ex, float ey, float ax, float ay, float tol) {
    TEST_ASSERT_FLOAT_WITHIN(tol, ex, ax);
    TEST_ASSERT_FLOAT_WITHIN(tol, ey, ay);
}

// --- Tests Instantiate_collider ---

void test_Instantiate_collider_origin(void) {
    Vector pos = {0.0f, 0.0f};
    Collider c = Instantiate_collider(pos, 10.0f, 10.0f);
    assert_vec(-5.0f, -5.0f, c.bounding_box[0].x, c.bounding_box[0].y, 0.0001f);
    assert_vec(5.0f, 5.0f, c.bounding_box[1].x, c.bounding_box[1].y, 0.0001f);
}

void test_Instantiate_collider_offset_position(void) {
    Vector pos = {20.0f, 30.0f};
    Collider c = Instantiate_collider(pos, 8.0f, 6.0f);
    assert_vec(16.0f, 27.0f, c.bounding_box[0].x, c.bounding_box[0].y, 0.0001f);
    assert_vec(24.0f, 33.0f, c.bounding_box[1].x, c.bounding_box[1].y, 0.0001f);
}

void test_Instantiate_collider_symmetric(void) {
    Vector pos = {50.0f, 80.0f};
    float w = 20.0f, h = 12.0f;
    Collider c = Instantiate_collider(pos, w, h);

    // Le centre doit correspondre a pos
    float cx = (c.bounding_box[0].x + c.bounding_box[1].x) / 2.0f;
    float cy = (c.bounding_box[0].y + c.bounding_box[1].y) / 2.0f;
    assert_vec(pos.x, pos.y, cx, cy, 0.0001f);

    // Largeur et hauteur correctes
    float box_w = c.bounding_box[1].x - c.bounding_box[0].x;
    float box_h = c.bounding_box[1].y - c.bounding_box[0].y;
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, w, box_w);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, h, box_h);
}

void test_Instantiate_collider_unit_size(void) {
    Vector pos = {5.0f, 5.0f};
    Collider c = Instantiate_collider(pos, 1.0f, 1.0f);
    assert_vec(4.5f, 4.5f, c.bounding_box[0].x, c.bounding_box[0].y, 0.0001f);
    assert_vec(5.5f, 5.5f, c.bounding_box[1].x, c.bounding_box[1].y, 0.0001f);
}

void test_Instantiate_collider_large_size(void) {
    Vector pos = {700.0f, 500.0f};
    Collider c = Instantiate_collider(pos, 1400.0f, 1000.0f);
    assert_vec(0.0f, 0.0f, c.bounding_box[0].x, c.bounding_box[0].y, 0.0001f);
    assert_vec(1400.0f, 1000.0f, c.bounding_box[1].x, c.bounding_box[1].y, 0.0001f);
}

void test_Instantiate_collider_non_square(void) {
    Vector pos = {100.0f, 200.0f};
    Collider c = Instantiate_collider(pos, 40.0f, 10.0f);
    assert_vec(80.0f, 195.0f, c.bounding_box[0].x, c.bounding_box[0].y, 0.0001f);
    assert_vec(120.0f, 205.0f, c.bounding_box[1].x, c.bounding_box[1].y, 0.0001f);
}

void test_Instantiate_collider_negative_position(void) {
    Vector pos = {-10.0f, -20.0f};
    Collider c = Instantiate_collider(pos, 6.0f, 4.0f);
    assert_vec(-13.0f, -22.0f, c.bounding_box[0].x, c.bounding_box[0].y, 0.0001f);
    assert_vec(-7.0f, -18.0f, c.bounding_box[1].x, c.bounding_box[1].y, 0.0001f);
}

void test_Instantiate_collider_float_dimensions(void) {
    Vector pos = {0.0f, 0.0f};
    Collider c = Instantiate_collider(pos, 3.5f, 7.25f);
    assert_vec(-1.75f, -3.625f, c.bounding_box[0].x, c.bounding_box[0].y, 0.0001f);
    assert_vec(1.75f, 3.625f, c.bounding_box[1].x, c.bounding_box[1].y, 0.0001f);
}

void test_Instantiate_collider_returns_copy(void) {
    Vector pos = {10.0f, 20.0f};
    Collider c1 = Instantiate_collider(pos, 8.0f, 6.0f);
    Collider c2 = Instantiate_collider(pos, 8.0f, 6.0f);

    // Memes valeurs
    assert_vec(c1.bounding_box[0].x, c1.bounding_box[0].y,
               c2.bounding_box[0].x, c2.bounding_box[0].y, 0.0001f);
    assert_vec(c1.bounding_box[1].x, c1.bounding_box[1].y,
               c2.bounding_box[1].x, c2.bounding_box[1].y, 0.0001f);

    // Modifier c1 n'affecte pas c2
    c1.bounding_box[0].x = 999.0f;
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 6.0f, c2.bounding_box[0].x);
}

// --- Tests random_float ---

void test_random_float_range(void) {
    // random_float(min, max) doit toujours etre dans [min, max]
    for (int i = 0; i < 100; i++) {
        float r = random_float(10.0f, 20.0f);
        TEST_ASSERT_TRUE(r >= 10.0f);
        TEST_ASSERT_TRUE(r <= 20.0f);
    }
}

void test_random_float_same_seed_same_result(void) {
    srand(42);
    float a = random_float(0.0f, 100.0f);

    srand(42);
    float b = random_float(0.0f, 100.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, a, b);
}

// --- Tests Colliders_random_array ---

void test_Colliders_random_array_count(void) {
    int count = 5;
    Collider *arr = Colliders_random_array(800, 600, 50, 50, count);
    TEST_ASSERT_NOT_NULL(arr);

    // Verifier que chaque collider est initialise (pas de valeurs indefinies)
    for (int i = 0; i < count; i++) {
        // bounding_box[0] (coin sup-gauche) doit etre <= bounding_box[1] (coin inf-droit)
        TEST_ASSERT_TRUE(arr[i].bounding_box[0].x <= arr[i].bounding_box[1].x);
        TEST_ASSERT_TRUE(arr[i].bounding_box[0].y <= arr[i].bounding_box[1].y);
    }
    Colliders_destroy_array(arr);
}

void test_Colliders_random_array_reproducible(void) {
    // Meme seed = meme resultat
    srand(123);
    Collider *a = Colliders_random_array(800, 600, 50, 50, 3);
    srand(123);
    Collider *b = Colliders_random_array(800, 600, 50, 50, 3);

    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(b);

    for (int i = 0; i < 3; i++) {
        assert_vec(a[i].bounding_box[0].x, a[i].bounding_box[0].y,
                   b[i].bounding_box[0].x, b[i].bounding_box[0].y, 0.0001f);
        assert_vec(a[i].bounding_box[1].x, a[i].bounding_box[1].y,
                   b[i].bounding_box[1].x, b[i].bounding_box[1].y, 0.0001f);
    }
    Colliders_destroy_array(a);
    Colliders_destroy_array(b);
}

void test_Colliders_random_array_different_seed(void) {
    // Seed different = resultats differents (quasi certainement)
    srand(1);
    Collider *a = Colliders_random_array(800, 600, 50, 50, 3);
    srand(2);
    Collider *b = Colliders_random_array(800, 600, 50, 50, 3);

    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(b);

    // Au moins un collider doit differer
    int same = 1;
    for (int i = 0; i < 3; i++) {
        if (a[i].bounding_box[0].x != b[i].bounding_box[0].x ||
            a[i].bounding_box[0].y != b[i].bounding_box[0].y) {
            same = 0;
            break;
        }
    }
    TEST_ASSERT_FALSE(same);
    Colliders_destroy_array(a);
    Colliders_destroy_array(b);
}

void test_Colliders_random_array_null_on_zero_count(void) {
    // count = 0 : comportement a definir, mais pas de crash
    Collider *arr = Colliders_random_array(800, 600, 50, 50, 0);
    // calloc(0, ...) peut retourner NULL ou un pointeur valide non utilisable
    // On verifie juste qu'on peut appeler destroy sans crash
    if (arr) {
        Colliders_destroy_array(arr);
    }
    // Pas d'erreur = succes
}

// --- Tests Colliders_copy_array ---

void test_Colliders_copy_array_basic(void) {
    Collider *orig = Colliders_random_array(800, 600, 50, 50, 3);
    TEST_ASSERT_NOT_NULL(orig);

    Collider *copy = Colliders_copy_array(orig, 3);
    TEST_ASSERT_NOT_NULL(copy);

    // Les valeurs doivent etre identiques
    for (int i = 0; i < 3; i++) {
        assert_vec(orig[i].bounding_box[0].x, orig[i].bounding_box[0].y,
                   copy[i].bounding_box[0].x, copy[i].bounding_box[0].y, 0.0001f);
        assert_vec(orig[i].bounding_box[1].x, orig[i].bounding_box[1].y,
                   copy[i].bounding_box[1].x, copy[i].bounding_box[1].y, 0.0001f);
    }

    // Mais ce sont des allocations differentes (deep copy)
    TEST_ASSERT_TRUE(orig != copy);

    Colliders_destroy_array(orig);
    Colliders_destroy_array(copy);
}

void test_Colliders_copy_array_independent(void) {
    // Modifier la copie ne doit pas affecter l'original
    Collider *orig = Colliders_random_array(800, 600, 50, 50, 2);
    Collider *copy = Colliders_copy_array(orig, 2);

    TEST_ASSERT_NOT_NULL(orig);
    TEST_ASSERT_NOT_NULL(copy);

    float orig_x = orig[0].bounding_box[0].x;
    copy[0].bounding_box[0].x = 999.0f;

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, orig_x, orig[0].bounding_box[0].x);

    Colliders_destroy_array(orig);
    Colliders_destroy_array(copy);
}

void test_Colliders_copy_array_null_input(void) {
    // Entree NULL doit retourner NULL (pas de crash)
    Collider *copy = Colliders_copy_array(NULL, 5);
    TEST_ASSERT_NULL(copy);
}

// --- Tests Colliders_destroy_array ---

void test_Colliders_destroy_array_null(void) {
    // Appeler destroy sur NULL ne doit pas crasher
    int ret = Colliders_destroy_array(NULL);
    TEST_ASSERT_EQUAL(-1, ret);
}

void test_Colliders_destroy_array_valid(void) {
    Collider *arr = Colliders_random_array(800, 600, 50, 50, 3);
    TEST_ASSERT_NOT_NULL(arr);
    int ret = Colliders_destroy_array(arr);
    TEST_ASSERT_EQUAL(0, ret);
}

// --- Point d'entree Unity ---

int main(void) {
    UNITY_BEGIN();

    // Instantiate_collider
    RUN_TEST(test_Instantiate_collider_origin);
    RUN_TEST(test_Instantiate_collider_offset_position);
    RUN_TEST(test_Instantiate_collider_symmetric);
    RUN_TEST(test_Instantiate_collider_unit_size);
    RUN_TEST(test_Instantiate_collider_large_size);
    RUN_TEST(test_Instantiate_collider_non_square);
    RUN_TEST(test_Instantiate_collider_negative_position);
    RUN_TEST(test_Instantiate_collider_float_dimensions);
    RUN_TEST(test_Instantiate_collider_returns_copy);

    // random_float
    RUN_TEST(test_random_float_range);
    RUN_TEST(test_random_float_same_seed_same_result);

    // Colliders_random_array
    RUN_TEST(test_Colliders_random_array_count);
    RUN_TEST(test_Colliders_random_array_reproducible);
    RUN_TEST(test_Colliders_random_array_different_seed);
    RUN_TEST(test_Colliders_random_array_null_on_zero_count);

    // Colliders_copy_array
    RUN_TEST(test_Colliders_copy_array_basic);
    RUN_TEST(test_Colliders_copy_array_independent);
    RUN_TEST(test_Colliders_copy_array_null_input);

    // Colliders_destroy_array
    RUN_TEST(test_Colliders_destroy_array_null);
    RUN_TEST(test_Colliders_destroy_array_valid);

    return UNITY_END();
}
