#include "shark_controller.h"
#include "render_sdl.h"

Vector Rules_center(Vector shark_pos, Vector center_of_mass, bool has_prey) {
    if (!has_prey) {
        return Vector_init();
    }

    Vector to_center = Vector_sub(center_of_mass, shark_pos);
    float dist = Vector_length(to_center);

    float intensity = dist / SHARK_VISION_RANGE;

    return Vector_scale(local_normalize(to_center), intensity);
}

Vector shark_choose_action(SharkPerception *perception) {
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
        // à faire
    }

    return action;
}