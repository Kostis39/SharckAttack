#include "shark_controller.h"
#include "render_sdl.h"
Vector shark_compute_mu(SharkTheta *theta, SharkPhi *phi) {
    Vector mu = Vector_init();

    if (theta == NULL || phi == NULL) {
        return mu;
    }

    mu.x = (1.0f * theta->biais) + (phi->center.x * theta->center) +
           (phi->alignment.x * theta->alignment) +
           (phi->pursuit.x * theta->pursuit);

    mu.y = (1.0f * theta->biais) + (phi->center.y * theta->center) +
           (phi->alignment.y * theta->alignment) +
           (phi->pursuit.y * theta->pursuit);

    return mu;
}

void box_muller_standard(float *x, float *y) {
    // 1. rand() / RAND_MAX donne un nombre uniforme entre 0 et 1 (noté rand(1)
    // dans l'algo)
    float u1 = (float)rand() / (float)RAND_MAX;
    float u2 = (float)rand() / (float)RAND_MAX;

    // Sécurité pour éviter log(0) qui tend vers l'infini -inf
    if (u1 < 1e-7f)
        u1 = 1e-7f;

    // 2. Application directe de l'algorithme fourni
    float t = 2.0f * (float)PI * u2; // Thêta : uniforme sur [0, 2π]
    float s = -2.0f * logf(u1);      // S = R^2 : suit une loi exponentielle

    // 3. Transformation en coordonnées cartésiennes pour obtenir X et Y
    *x = sqrtf(s) * cosf(t);
    *y = sqrtf(s) * sinf(t);
}

Vector Rules_center(Shark shark, Vector center_of_mass, bool has_prey_visible) {
    if (!has_prey_visible) {
        return Vector_init();
    }

    Vector to_center = Vector_sub(center_of_mass, shark.pos);
    float dist = Vector_length(to_center);

    // Plus le centre est loin, plus la correction est forte
    float intensity = dist / shark.radius_vision;

    return Vector_scale(local_normalize(to_center), intensity);
}

Vector Rules_alignment_shark(Shark shark, Vector avg_velocity,
                             bool has_prey_visible) {
    if (!has_prey_visible) {
        return Vector_init();
    }

    // Différence entre la vitesse moyenne et la vitesse du requin
    Vector diff = Vector_sub(avg_velocity, shark.velocity);
    float norm = Vector_length(diff);

    // Plus l'écart est grand, plus la correction est forte
    float intensity = norm / SHARK_SPEED_MAX;

    return Vector_scale(local_normalize(diff), intensity);
}

Vector Rules_pursuit(Shark shark, Vector fish_pos, bool has_prey, int width,
                     int height) {
    if (!has_prey) {
        return Vector_init();
    }

    Vector to_fish = Vector_sub(fish_pos, shark.pos);
    float dist = Vector_length(to_fish);

    // Plus le poisson est proche, plus on va vers le poisson
    Vector max = {width * width, height * height};
    float intensity = 1.0f / (1.0f + (dist / Vector_length(max)));

    return Vector_scale(local_normalize(to_fish), intensity);
}

Vector shark_choose_action(SharkPerception *perception, SharkTheta *theta) {
    if (!perception)
        return Vector_init();

    Vector action = Vector_init();
    SharkPhi phi = {0};

    if (Is_player(&perception->self)) { // Mode joueur
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        Vector target = {(float)mx, (float)my};
        Vector to_target = Vector_sub(target, perception->self.pos);

        float dist = Vector_length(to_target);

        Vector direction = local_normalize(to_target);
        // Intensité proportionnelle à la distance
        float intensity = dist / SHARK_SPEED_MAX;
        if (intensity > 1.0f)
            intensity = 1.0f;
        action = Vector_scale(direction, intensity);

    } else { // Mode bot
        phi.center = Rules_center(perception->self, perception->center_of_mass,
                                  perception->has_prey_visible);

        phi.alignment =
            Rules_alignment_shark(perception->self, perception->avg_velocity,
                                  perception->has_prey_visible);

        phi.pursuit = Rules_pursuit(
            perception->self, perception->closest_fish.position,
            perception->has_prey, perception->width, perception->height);

        // 4. Combinaison pondérée des vecteurs
        Vector mu = shark_compute_mu(theta, &phi);

        float noise_x, noise_y;
        box_muller_standard(&noise_x, &noise_y);

        action.x = mu.x + noise_x * 5;
        action.y = mu.y + noise_y * 5;
    }

    return action;
}
