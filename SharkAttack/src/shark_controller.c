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

    return Vector_scale(Local_normalize(to_center), intensity);
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

    // Plus l'écart est grand, plus la correction est forte
    float intensity = norm / SHARK_SPEED_MAX;

    return Vector_scale(Local_normalize(diff), intensity);
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

    return Vector_scale(Local_normalize(to_fish), intensity);
}

/**
 * @brief règle d'orientation vers le centre de masse de la zone
 *        directionnelle la plus dense (avant / arrière / gauche / droite)
 * @param shark structure du requin
 * @param perception perception du requin
 * @return vecteur de densité de zone
 * @note Principe : on calcule un score par zone (nombre de poissons pondéré par
 * la proximité du centre de masse de la zone), on identifie la meilleure zone,
 * puis on retourne un vecteur vers le centre de masse de cette zone
 */
Vector Rules_zone_density_shark(Shark shark, SharkPerception perception) {

    float scores[Count];
    float total_score = 0.0f;
    bool found_zone = false;
    float best_score = -1.0f;
    ZoneDirection best_zone;

    Vector diag = {perception.width, perception.height};
    float dist_max = Vector_length(diag);

    for (int z = 0; z < Count; z++) {
        if (!perception.zone_has_prey_visible[z]) {
            scores[z] = 0.0f;
            continue;
        }

        // Distance du centre de masse de la zone
        Vector to_zone_center =
            Vector_sub(perception.zone_center_of_mass[z], shark.pos);
        float dist = Vector_length(to_zone_center);

        // Score : plus de poissons + plus proches => meilleur score
        scores[z] = (float)perception.zone_count[z] / (1.0f + dist / dist_max);

        total_score += scores[z];

        if (scores[z] > best_score) {
            best_score = scores[z];
            best_zone = (ZoneDirection)z;
            found_zone = true;
        }
    }

    if (!found_zone || total_score < 1e-6f) {
        // Aucune zone avec des poissons visibles
        return Vector_init();
    }

    // Direction réelle vers le centre de masse de la meilleure zone
    Vector to_best_center =
        Vector_sub(perception.zone_center_of_mass[best_zone], shark.pos);
    float dist = Vector_length(to_best_center);

    // Intensité proportionnelle à la distance
    float intensity = dist / dist_max;

    return Vector_scale(Vector_normalize(to_best_center), intensity);
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

        Vector zone_density =
            Rules_zone_density_shark(perception->self, *perception);
        SharkPhi_add_vector(&phi, zone_density, Rules_zone_density);

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
