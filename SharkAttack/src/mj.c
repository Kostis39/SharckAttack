#include "mj.h"

/**
 * @brief calcule la récompense du requin en fonction du nombre de poissons
 * mangés
 * @param nb_fish_ate nombre de poissons mangés lors de cette itération
 * @return valeur de la récompense
 */
float Shark_reward(int nb_fish_ate) { return nb_fish_ate; }

void fish_perception_init(Fish *fish, World *world,
                          FishPerception *perception) {
    perception->self = *fish;

    perception->collider_separation = Vector_init();
    perception->separation = Vector_init();
    perception->center_of_mass = Vector_init();
    perception->avg_velocity = Vector_init();
    perception->dist_shark = Vector_init();

    perception->shark_visible = false;
    perception->width = world->width;
    perception->height = world->height;
}

/**
 * @brief Met à jour la perception du requin pour un poisson
 * @param fish le poisson dont on met à jour la perception
 * @param world le monde contenant le requin
 * @param perception la structure de perception à mettre à jour
 */
void fish_to_shark_perception(Fish *fish, World *world,
                              FishPerception *perception) {
    float shark_closest_dist = INFINITY;
    float dist = 0;

    for (int i = 0; i < world->nb_sharks; ++i) {
        if (world->sharks[i] != NULL) {
            Vector to_shark = Vector_sub(fish->position, world->sharks[i]->pos);
            dist = Vector_length(to_shark);

            if (dist < RADIUS_SHARK_VISIBILITY && dist < shark_closest_dist) {
                perception->shark_visible = true;
                perception->shark = *world->sharks[i];
            }
        }
    }
    if (perception->shark_visible) {
        perception->dist_shark =
            Vector_sub(perception->self.position, perception->shark.pos);
    }
}

/**
 * @brief fish_neighbor_perception donne la percpetion des voisins de fish
 * @param fish le poisson
 * @param world le monde
 * @param perception la perception actuelle du poisson
 * @param nb_align pointeur vers le nombre de poissons dans la zone
 * d'alignement
 * @param nb_align pointeur vers le nombre de poissons dans la zone de
 * cohésion
 */
void fish_neighbor_perception(Fish *fish, World *world,
                              FishPerception *perception, int *nb_align,
                              int *nb_cohes) {
    float separation_limit = fish->radius_separation;
    float alignment_limit = separation_limit + fish->radius_alignement;
    float cohesion_limit = alignment_limit + fish->radius_cohesion;
    for (int i = 0; i < world->nb_fish; i++) {
        Fish *neighbor = &world->fishes[i];

        if (neighbor == fish || !neighbor->is_alive) {
            continue;
        }

        Vector to_neighbor = Vector_sub(neighbor->position, fish->position);
        float distance_squared = Vector_length2(to_neighbor);

        float angle = Vector_angle(fish->velocity, to_neighbor);
        if (fabsf(angle) > fish->vision_angle / 2.0f) {
            continue;
        }

        if (distance_squared <= separation_limit * separation_limit) {
            perception->separation = Vector_add(
                perception->separation,
                Vector_sub(perception->self.position, neighbor->position));
        } else if (distance_squared <= alignment_limit * alignment_limit) {
            perception->avg_velocity =
                Vector_add(perception->avg_velocity, neighbor->velocity);
            (*nb_align)++;
        } else if (distance_squared <= cohesion_limit * cohesion_limit) {
            perception->center_of_mass =
                Vector_add(perception->center_of_mass, neighbor->position);
            (*nb_cohes)++;
        }
    }
}

/**
 * @brief renvoie la perception d'un poisson dans le monde
 * @param fish le poisson en question
 * @param world le monde dans lequel évolue le poisson
 * @param perception la struct dans laquelle on écrit la perception associée
 * au poisson fish
 * */
void get_fish_perception(Fish *fish, World *world, FishPerception *perception) {
    fish_perception_init(fish, world, perception);
    fish_to_shark_perception(fish, world, perception);
    int nb_cohes = 0;
    int nb_align = 0;
    fish_neighbor_perception(fish, world, perception, &nb_align, &nb_cohes);

    /**Les trois zones de perception sont concentriques et s'enchaînent :
     * [0, separation_limit] -> séparation
     * ]separation_limit, alignment_limit] -> alignement
     * ]alignment_limit, cohesion_limit] -> cohésion
     */

    float max_perception = fish->radius_separation;

    for (int j = 0; j < world->nb_colliders; ++j) {
        Collider *c = &world->colliders[j];
        Vector closest = Collider_closest_point(c, fish->position);
        float dist = Vector_distance(fish->position, closest);

        if (dist < max_perception && dist > 0.0f) {
            float intensity = 1.0f - (dist / max_perception);
            Vector repulsion =
                Vector_scale(Vector_sub(fish->position, closest), intensity);
            perception->collider_separation =
                Vector_add(perception->collider_separation, repulsion);
        }
    }
    perception->avg_velocity =
        (nb_align > 0) ? Vector_scale(perception->avg_velocity, 1.0f / nb_align)
                       : Vector_init();

    perception->center_of_mass =
        (nb_cohes > 0)
            ? Vector_scale(perception->center_of_mass, 1.0f / nb_cohes)
            : perception->self.position;
}

/**
 * @brief libère la mémoire d'une structure FishPerception
 * @param perception pointeur vers la perception à libérer
 */
void FishPerception_destroy(FishPerception *perception) { free(perception); }

/**
 * @brief détermine dans quelle zone directionnelle se trouve un poisson
 * par rapport à l'orientation du requin
 * @param shark structure du requin
 * @param fish_pos position du poisson
 * @return la zone correspondante (Front, Back, Left, Right)
 */
ZoneDirection Shark_get_fish_zone(Shark shark, Vector fish_pos) {
    Vector forward = Local_normalize(shark.velocity); // (1,0) si vitesse nulle
    Vector right = {-forward.y, forward.x};           // rotation 90° à droite
    Vector to_fish = Vector_sub(fish_pos, shark.pos);

    float f = Vector_dot(to_fish, forward);
    float r = Vector_dot(to_fish, right);

    if (fabsf(f) >= fabsf(r))
        return (f >= 0.0f) ? Front : Back;
    else
        return (r >= 0.0f) ? Right : Left;
}

/**
 * @brief construit la perception du requin à partir du monde
 * @param shark pointeur vers le requin
 * @param world pointeur vers le monde
 * @param shark_perception pointeur vers la structure à remplir
 */
void Get_shark_perception(Shark *shark, World *world,
                          SharkPerception *shark_perception) {

    shark_perception->self = *shark;

    shark_perception->width = world->width;
    shark_perception->height = world->height;

    Vector center = Vector_init();
    Vector avg_vel = Vector_init();
    int count = 0;

    float fish_closest_dist = INFINITY;
    float dist = 0;

    Vector zone_center[Count];
    Vector zone_vel[Count];
    int zone_count[Count];

    for (int z = 0; z < Count; z++) {
        zone_center[z] = Vector_init();
        zone_vel[z] = Vector_init();
        zone_count[z] = 0;
    }

    shark_perception->nb_fish_remaining = 0;

    // Parcourir tous les poissons
    for (int i = 0; i < world->nb_fish; i++) {
        Fish *fish = &world->fishes[i];
        if (!fish->is_alive)
            continue;

        // Distance au requin
        Vector to_fish = Vector_sub(fish->position, shark->pos);
        dist = Vector_length(to_fish);

        // Mettre à jour le poisson le plus proche
        if (dist < fish_closest_dist) {
            fish_closest_dist = dist;
            shark_perception->closest_fish = *fish;
        }

        // Calculs pour les poisson proche
        if (dist < SHARK_VISION_RANGE) {
            center = Vector_add(center, fish->position);
            avg_vel = Vector_add(avg_vel, fish->velocity);
            count++;
        }

        ZoneDirection zone = Shark_get_fish_zone(*shark, fish->position);
        zone_center[zone] = Vector_add(zone_center[zone], fish->position);
        zone_vel[zone] = Vector_add(zone_vel[zone], fish->velocity);
        zone_count[zone]++;
    }

    if (count > 0) {
        shark_perception->center_of_mass = Vector_scale(center, 1.0f / count);
        shark_perception->avg_velocity = Vector_scale(avg_vel, 1.0f / count);

        shark_perception->has_prey_visible = true;

    } else { // Aucun poisson visible
        shark_perception->center_of_mass = Vector_init();
        shark_perception->avg_velocity = Vector_init();

        shark_perception->has_prey_visible = false;
    }
    shark_perception->has_prey = fish_closest_dist != INFINITY;

    // moyenne des 4 zones
    for (int z = 0; z < Count; z++) {
        shark_perception->zone_count[z] = zone_count[z];
        shark_perception->nb_fish_remaining += shark_perception->zone_count[z];

        if (zone_count[z] > 0) {
            shark_perception->zone_center_of_mass[z] =
                Vector_scale(zone_center[z], 1.0f / zone_count[z]);
            shark_perception->zone_avg_velocity[z] =
                Vector_scale(zone_vel[z], 1.0f / zone_count[z]);
            shark_perception->zone_has_prey_visible[z] = true;
        } else {
            shark_perception->zone_center_of_mass[z] = Vector_init();
            shark_perception->zone_avg_velocity[z] = Vector_init();
            shark_perception->zone_has_prey_visible[z] = false;
        }
    }

    float shark_closest_dist = INFINITY;
    Vector to_shark = shark->pos;

    for (int i = 0; i < world->nb_sharks; ++i) {
        if (world->sharks[i] != shark) {
            to_shark = Vector_sub(world->sharks[i]->pos, shark->pos);
            dist = Vector_length(to_shark);

            if (dist < shark_closest_dist) {
                shark_closest_dist = dist;
                shark_perception->closest_shark = world->sharks[i]->pos;
            }
        }
    }
    shark_perception->closest_shark = to_shark;
}

/**
 * @brief Calcule combien de poissons sont mangés, et les mange.
 *
 * @param world Le monde où ce passe l'action.
 * @return int Le nombre de poisson mangé.
 */
int handle_shark_eat(World *world, int shark_idx) {
    Shark *shark = world->sharks[shark_idx];
    Vector shark_pos = shark->pos;
    int eaten = 0;

    for (int i = 0; i < world->nb_fish; i++) {
        Fish *fish = &world->fishes[i];
        if (!fish->is_alive)
            continue;

        float dist = Vector_length(Vector_sub(fish->position, shark_pos));
        if (dist < SHARK_ATTACK_RANGE) {
            fish->is_alive = false;
            world->fish_eaten++;
            eaten++;
        }
    }
    return eaten;
}

void UpdateWorld(World *world, World *tmp_world) {
    FishPerception *perception = malloc(sizeof(FishPerception));
    Vector shark_action;
    StepTrajectory *step_trajectory;

    if (!perception)
        return;

    for (int i = 0; i < world->nb_fish; i++) {
        // Copie de l'état actuel
        tmp_world->fishes[i] = world->fishes[i];

        if (!world->fishes[i].is_alive) {
            continue;
        }

        get_fish_perception(&world->fishes[i], world, perception);

        Vector action = fish_choose_action(perception, world->theta_fish,
                                           world->width, world->height);

        fish_apply_action(&tmp_world->fishes[i], action);
    }
    FishPerception_destroy(perception);

    // Copie de l'état actuel
    *tmp_world->sharks = *world->sharks;
    int nb_fish_ate = 0;

    SharkPerception *shark_perception = malloc(sizeof(SharkPerception));
    if (!shark_perception)
        return;

    for (int s = 0; s < world->nb_sharks; s++) {
        Get_shark_perception(world->sharks[s], world, shark_perception);

        if (world->learn) {
            Need_trajectory_growing(world->trajectory);
            step_trajectory =
                &world->trajectory->steps[world->trajectory->length];

            shark_action =
                shark_choose_action(shark_perception, world->theta_shark,
                                    step_trajectory, world->sigma);
        } else {
            shark_action = shark_choose_action(
                shark_perception, world->theta_shark, NULL, world->sigma);
        }
        Shark_apply_action(tmp_world->sharks[s], shark_action, world->width,
                           world->height);

        nb_fish_ate += handle_shark_eat(tmp_world);
    }
    free(shark_perception);

    if (world->learn) {
        step_trajectory->reward = Shark_reward(nb_fish_ate);
        world->trajectory->length++;
    }
}

void Game_step(World *world) {
    if (!world || world->nb_fish < 0) {
        fprintf(stderr, "Game_step: monde invalide\n");
        return;
    }

    World tmp_world;

    // Création du monde temporaire
    if (!World_create_tmp(world, &tmp_world)) {
        return;
    }

    // Mise à jour des perceptions et actions
    UpdateWorld(world, &tmp_world);

    // Échange des données
    World_swap_data(world, &tmp_world);
}
