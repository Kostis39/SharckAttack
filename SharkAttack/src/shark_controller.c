#include "shark_controller.h"
#include "render_sdl.h"

/**
 * @brief règle d'attraction vers le centre de masse des poissons visibles
 * @param shark structure du requin
 * @param center_of_mass centre de masse des poissons visibles
 * @param has_prey_visible vrai si au moins un poisson est dans le champ de
 * vision
 * @return vecteur de centre de masse
 */
Vector Rules_center(Shark shark, Vector center_of_mass, bool has_prey_visible) {
    if (!has_prey_visible) {
        return Vector_init();
    }

    Vector to_center = Vector_sub(center_of_mass, shark.pos);
    float dist = Vector_length(to_center);

    // Plus le centre est loin, plus la correction est forte
    float intensity = dist / shark.radius_vision;
    if (intensity > 1.0f)
        intensity = 1.0f;
    return Vector_scale(Vector_normalize(to_center), intensity);
}

/**
 * @brief règle d'alignement du requin sur la direction moyenne des poissons
 * @param shark structure du requin
 * @param avg_velocity direction moyenne des poissons visibles
 * @param has_prey_visible vrai si au moins un poisson est dans le champ de
 * vision
 * @return vecteur d'alignement
 */
Vector Rules_alignment_shark(Shark shark, Vector avg_velocity,
                             bool has_prey_visible) {
    if (!has_prey_visible) {
        return Vector_init();
    }

    // Différence entre la vitesse moyenne et la vitesse du requin
    Vector diff = Vector_sub(avg_velocity, shark.velocity);

    float norm = Vector_length(diff);
    float max_norm = 2.0f * SHARK_SPEED_MAX;
    float intensity = norm / max_norm;
    if (intensity > 1.0f)
        intensity = 1.0f;

    return Vector_normalize(diff);
}

/**
 * @brief règle de poursuite vers le poisson la plus proche
 * @param shark structure du requin
 * @param fish_pos position de le poisson la plus proche
 * @param has_prey vrai si il existe un poisson
 * @param width largeur du monde
 * @param height hauteur du monde
 * @return vecteur de poursuite
 */
Vector Rules_pursuit(Shark shark, Vector fish_pos, bool has_prey, int width,
                     int height) {
    if (!has_prey) {
        return Vector_init();
    }

    Vector to_fish = Vector_sub(fish_pos, shark.pos);
    float dist = Vector_length(to_fish);

    // Plus le poisson est proche, plus on va vers le poisson
    Vector max = {width, height};
    float intensity = 1.0f / (1.0f + (dist / Vector_length(max)));
    if (intensity > 1.0f)
        intensity = 1.0f;

    return Vector_scale(Vector_normalize(to_fish), intensity);
}

/**
 * @brief vecteur directionnel pondéré pour la zone devant le requin
 * @param shark structure du requin
 * @param perception perception du requin
 * @return vecteur dans la direction devant le requin, pondéré par le ratio de
 * poissons dans la zone
 */
Vector Rules_front(Shark shark, SharkPerception perception) {
    if (!perception.zone_has_prey_visible[Front] ||
        perception.nb_fish_remaining == 0) {
        return Vector_init();
    }
    float ratio = (float)perception.zone_count[Front] /
                  (float)perception.nb_fish_remaining;
    Vector forward = Vector_normalize(shark.velocity);

    return Vector_scale(forward, ratio);
}

/**
 * @brief vecteur directionnel pondéré pour la zone arrière
 */
Vector Rules_back(Shark shark, SharkPerception perception) {
    if (!perception.zone_has_prey_visible[Back] ||
        perception.nb_fish_remaining == 0) {
        return Vector_init();
    }
    float ratio = (float)perception.zone_count[Back] /
                  (float)perception.nb_fish_remaining;
    Vector forward = Vector_normalize(shark.velocity);

    return Vector_scale(Vector_scale(forward, -1.0f), ratio);
}

/**
 * @brief vecteur directionnel pondéré pour la zone gauche
 */
Vector Rules_left(Shark shark, SharkPerception perception) {
    if (!perception.zone_has_prey_visible[Left] ||
        perception.nb_fish_remaining == 0) {
        return Vector_init();
    }
    float ratio = (float)perception.zone_count[Left] /
                  (float)perception.nb_fish_remaining;
    Vector forward = Vector_normalize(shark.velocity);
    Vector left = {forward.y, -forward.x};

    return Vector_scale(left, ratio);
}

/**
 * @brief vecteur directionnel pondéré pour la zone droite
 */
Vector Rules_right(Shark shark, SharkPerception perception) {
    if (!perception.zone_has_prey_visible[Right] ||
        perception.nb_fish_remaining == 0) {
        return Vector_init();
    }
    float ratio = (float)perception.zone_count[Right] /
                  (float)perception.nb_fish_remaining;
    Vector forward = Vector_normalize(shark.velocity);
    Vector right = {-forward.y, forward.x};

    return Vector_scale(right, ratio);
}

Vector shark_choose_action(SharkPerception *perception, VectorRule theta,
                           StepTrajectory *step_trajectory, float sigma) {
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

        Vector direction = Local_normalize(to_target);

        // Intensité proportionnelle à la distance
        float intensity = dist / 20.0f;
        if (intensity > 1.0f)
            intensity = 1.0f;
        action = Vector_scale(direction, intensity * SHARK_SPEED_MAX);

    } else { // Mode bot
        Vector center =
            Rules_center(perception->self, perception->center_of_mass,
                         perception->has_prey_visible);
        SharkPhi_add_vector(&phi, center, Rules_Center);

        Vector alignment =
            Rules_alignment_shark(perception->self, perception->avg_velocity,
                                  perception->has_prey_visible);
        SharkPhi_add_vector(&phi, alignment, Rules_Alignment);

        Vector pursuit = Rules_pursuit(
            perception->self, perception->closest_fish.position,
            perception->has_prey, perception->width, perception->height);
        SharkPhi_add_vector(&phi, pursuit, Rules_Pursuit);

        Vector front = Rules_front(perception->self, *perception);
        Vector back = Rules_back(perception->self, *perception);
        Vector left = Rules_left(perception->self, *perception);
        Vector right = Rules_right(perception->self, *perception);

        SharkPhi_add_vector(&phi, front, Rules_zone_Front);
        SharkPhi_add_vector(&phi, back, Rules_zone_Back);
        SharkPhi_add_vector(&phi, left, Rules_zone_Left);
        SharkPhi_add_vector(&phi, right, Rules_zone_Right);

        // Combinaison pondérée des vecteurs
        Vector mu;
        mu.x = VectorRule_dot_product(theta, phi.x);
        mu.y = VectorRule_dot_product(theta, phi.y);

        Vector noise = box_muller_standard();

        action.x = mu.x + noise.x * sigma;
        action.y = mu.y + noise.y * sigma;

        if (step_trajectory) {
            step_trajectory->phi = phi;
            step_trajectory->action = action;
        }
    }

    return action;
}
