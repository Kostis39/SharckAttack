#include "shark_controller.h"
#include "render_sdl.h"

Vector Rules_center(Shark shark, Vector center_of_mass, bool has_prey) {
    if (!has_prey) {
        return Vector_init();
    }

    Vector to_center = Vector_sub(center_of_mass, shark.pos);
    float dist = Vector_length(to_center);

    // Plus le centre est loin, plus la correction est forte
    float intensity = dist / shark.radius_vision;

    return Vector_scale(local_normalize(to_center), intensity);
}

Vector Rules_alignment_shark(Vector shark_vel, Vector avg_velocity,
                             bool has_prey) {
    if (!has_prey) {
        return Vector_init();
    }

    // Différence entre la vitesse moyenne et la vitesse du requin
    Vector diff = Vector_sub(avg_velocity, shark_vel);
    float norm = Vector_length(diff);

    // Plus l'écart est grand, plus la correction est forte
    float intensity = norm / SHARK_SPEED_MAX;

    return Vector_scale(local_normalize(diff), intensity);
}

Vector Rules_pursuit(Vector shark_pos, Vector fish_pos, bool has_prey,
                     int width, int height) {
    if (!has_prey) {
        return Vector_init();
    }

    Vector to_fish = Vector_sub(fish_pos, shark_pos);
    float dist = Vector_length(to_fish);

    // Plus le poisson est proche, plus on va vers le poisson
    Vector max = {width * width, height * height};
    float intensity = 1.0f / (1.0f + (dist / Vector_length(max)));

    return Vector_scale(local_normalize(to_fish), intensity);
}

Vector shark_choose_action(SharkPerception *perception,
                           RulesSetShark *rules_set) {
    if (!perception)
        return Vector_init();

    Vector action = Vector_init();

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
        Vector center = Rules_center(
            perception->self, perception->center_of_mass, perception->has_prey);

        Vector alignment = Rules_alignment_shark(perception->self.velocity,
                                                 perception->avg_velocity,
                                                 perception->has_prey);

        Vector pursuit = Rules_pursuit(
            perception->self.pos, perception->closest_fish.position,
            perception->has_prey, perception->width, perception->height);

        Vector_print(Vector_scale(center, rules_set->center));
        Vector_print(Vector_scale(alignment, rules_set->alignment));
        Vector_print(Vector_scale(pursuit, rules_set->pursuit));

        // 4. Combinaison pondérée des vecteurs
        action = Vector_add(action, Vector_scale(center, rules_set->center));
        action =
            Vector_add(action, Vector_scale(alignment, rules_set->alignment));
        action = Vector_add(action, Vector_scale(pursuit, rules_set->pursuit));

        Vector_print(action);
        printf("------\n");
    }

    return action;
}